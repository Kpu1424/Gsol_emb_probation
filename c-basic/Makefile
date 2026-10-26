# c-basic - make L01 (hoặc L02..L05): chạy một bài | make all: chạy cả 5 bài | make clean: dọn
L01:
	$(MAKE) -C L01_functions
L02:
	$(MAKE) -C L02_multifile
L03:
	$(MAKE) -C L03_arrays_pointers
L04:
	$(MAKE) -C L04_struct_union_bitfield
L05:
	$(MAKE) -C L05_io

all: L01 L02 L03 L04 L05

clean:
	$(MAKE) -C L01_functions clean
	$(MAKE) -C L02_multifile clean
	$(MAKE) -C L03_arrays_pointers clean
	$(MAKE) -C L04_struct_union_bitfield clean
	$(MAKE) -C L05_io clean

.PHONY: all clean L01 L02 L03 L04 L05
