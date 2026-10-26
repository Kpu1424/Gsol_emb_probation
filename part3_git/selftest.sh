#!/usr/bin/env bash
# Tự kiểm tra: chạy TỰ ĐỘNG các lệnh trong G1..G4 trong một thư mục TẠM (mktemp),
# kiểm tra kết quả và in PASS/FAIL. Không đụng tới sandbox của bạn hay repo nào khác.
# Mục đích: xác nhận các lệnh trong bài chạy đúng với phiên bản git trên máy.
#
#   ./selftest.sh
set -euo pipefail

TMP="$(mktemp -d)"
trap 'rm -rf -- "$TMP"' EXIT
export GIT_CONFIG_GLOBAL=/dev/null      # bỏ qua cấu hình global của máy
export GIT_AUTHOR_NAME="Selftest" GIT_AUTHOR_EMAIL="selftest@example.invalid"
export GIT_COMMITTER_NAME="Selftest" GIT_COMMITTER_EMAIL="selftest@example.invalid"

PASS=0; FAIL=0
check() {   # check "mo ta" lenh...
  local desc="$1"; shift
  if "$@" >/dev/null 2>&1; then echo "PASS  $desc"; PASS=$((PASS+1));
  else echo "FAIL  $desc"; FAIL=$((FAIL+1)); fi
}

echo "git: $(git --version)"
echo "thu muc tam: $TMP"
cd "$TMP"

# ---------- G1: init, status, add, commit, diff, log ----------
git init -q -b main sensor-app && cd sensor-app
printf '#define TEMP_OFFSET 0\nint read_temp(void) { return 250 + TEMP_OFFSET; }\n' > sensor.c
check "G1 status thay file chua theo doi" bash -c 'git status --short | grep -q "^?? sensor.c"'
git add sensor.c
check "G1 add -> staged" bash -c 'git status --short | grep -q "^A  sensor.c"'
git commit -q -m "Them sensor.c"
echo '/* doc nhiet do */' >> sensor.c
check "G1 diff thay thay doi chua stage" bash -c 'git diff | grep -q "doc nhiet do"'
git add sensor.c
check "G1 diff --staged thay thay doi da stage" bash -c 'git diff --staged | grep -q "doc nhiet do"'
git commit -q -m "Them chu thich"
check "G1 log co 2 commit" test "$(git log --oneline | wc -l)" -eq 2

# ---------- G2: branch, merge, conflict ----------
git switch -q -c feature/readme
echo "# Sensor app" > README.md
git add README.md && git commit -q -m "Them README"
git switch -q main
git merge -q feature/readme
check "G2 merge fast-forward co README" test -f README.md

git switch -q -c feature/calib
sed -i 's/TEMP_OFFSET 0/TEMP_OFFSET -3/' sensor.c
git commit -q -am "Hieu chuan offset -3"
git switch -q main
sed -i 's/TEMP_OFFSET 0/TEMP_OFFSET 5/' sensor.c
git commit -q -am "Offset 5 theo phan cung moi"
check "G2 merge bi conflict" bash -c '! git merge feature/calib'
check "G2 file co dau conflict" grep -q '^<<<<<<<' sensor.c
check "G2 status bao 'both modified'" bash -c 'git status | grep -q "both modified"'
sed -i '/^<<<<<<<\|^=======\|^>>>>>>>/d; /TEMP_OFFSET -3/d' sensor.c   # giữ bản của main (5)
git add sensor.c && git commit -q --no-edit
check "G2 da giai quyet conflict" bash -c 'grep -q "TEMP_OFFSET 5" sensor.c && ! grep -q "<<<<<<<" sensor.c'
check "G2 merge commit co 2 cha" test "$(git rev-list --parents -n1 HEAD | wc -w)" -eq 3

# ---------- G3: remote, push, clone, fetch, pull ----------
git init -q --bare -b main "$TMP/remote/sensor-app.git"
git remote add origin "$TMP/remote/sensor-app.git"
git push -q -u origin main
check "G3 push len bare repo" bash -c "git -C '$TMP/remote/sensor-app.git' log --oneline main | grep -q ."
git clone -q "$TMP/remote/sensor-app.git" "$TMP/teammate"
( cd "$TMP/teammate" && echo "v1" > VERSION && git add VERSION && git commit -q -m "Them VERSION" && git push -q )
git fetch -q
check "G3 fetch: origin/main di truoc main 1 commit" test "$(git rev-list --count main..origin/main)" -eq 1
check "G3 fetch chua doi working tree" test ! -f VERSION
git pull -q --no-rebase
check "G3 pull da co VERSION" test -f VERSION
( cd "$TMP/teammate" && echo "note" > NOTES && git add NOTES && git commit -q -m "Them NOTES" && git push -q )
echo "x" > LOCAL && git add LOCAL && git commit -q -m "Them LOCAL"
check "G3 push bi tu choi (non-fast-forward)" bash -c '! git push -q'
git pull -q --no-rebase --no-edit
check "G3 pull roi push thanh cong" git push -q

# ---------- G4: restore, revert, reset, stash, reflog ----------
echo "rac" >> sensor.c
git restore sensor.c
check "G4 restore bo thay doi working tree" bash -c '! grep -q rac sensor.c'
echo "rac" >> sensor.c && git add sensor.c
git restore --staged sensor.c
check "G4 restore --staged: unstage nhung giu noi dung" bash -c 'git status --short | grep -q "^ M sensor.c" && grep -q rac sensor.c'
git restore sensor.c

echo "bug" > BUG && git add BUG && git commit -q -m "Commit loi"
git revert --no-edit HEAD >/dev/null
check "G4 revert tao commit dao nguoc" bash -c 'test ! -f BUG && git log -1 --format=%s | grep -q "^Revert"'

BEFORE=$(git rev-parse HEAD)
echo a > A && git add A && git commit -q -m "A"
git reset -q --soft HEAD~1
check "G4 reset --soft: thay doi con trong staging" bash -c 'git status --short | grep -q "^A  A"'
git commit -q -m "A lai"
git reset -q --mixed HEAD~1
check "G4 reset --mixed: thay doi con trong working tree" bash -c 'git status --short | grep -q "^?? A"'
git add A && git commit -q -m "A lan 3"
git reset -q --hard "$BEFORE"
check "G4 reset --hard: mat file A" test ! -f A
REFLOG=$(git reflog)
LOST=$(grep -m1 'A lan 3' <<<"$REFLOG" | cut -d' ' -f1)
git reset -q --hard "$LOST"
check "G4 reflog phuc hoi commit sau reset --hard" test -f A

echo "dang lam do" >> sensor.c
git stash push -q -m "wip offset"
check "G4 stash: working tree sach" test -z "$(git status --short)"
check "G4 stash list co 1 muc" test "$(git stash list | wc -l)" -eq 1
git stash pop -q
check "G4 stash pop: thay doi quay lai" grep -q "dang lam do" sensor.c

echo
echo "Ket qua: $PASS PASS, $FAIL FAIL"
[ "$FAIL" -eq 0 ]
