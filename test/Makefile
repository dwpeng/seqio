
all: $(ROOT_DIR)/test-seqio $(ROOT_DIR)/test-kseq

$(ROOT_DIR)/test-seqio: test-seqio.c $(seqioLIB)
	$(cc) $(CFLAGS) -o $@ $^ $(LIBS)

$(ROOT_DIR)/test-kseq: test-kseq.c kseq.h
	$(cc) $(CFLAGS) -o $@ $^ $(LIBS)
