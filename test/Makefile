
all: $(ROOT_DIR)/test-seqio $(ROOT_DIR)/test-kseq $(ROOT_DIR)/test-seqio-stdin

$(ROOT_DIR)/test-seqio: test-seqio.c $(seqioObj)
	$(cc) $(CFLAGS) -o $@ $^ $(LIBS)

$(ROOT_DIR)/test-kseq: test-kseq.c kseq.h
	$(cc) $(CFLAGS) -o $@ $^ $(LIBS)

$(ROOT_DIR)/test-seqio-stdin: test-seqio-stdin.c $(seqioObj)
	$(cc) $(CFLAGS) -o $@ $^ $(LIBS)
