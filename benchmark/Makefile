all: $(ROOT_DIR)/benchmark-seqio $(ROOT_DIR)/benchmark-kseq

$(ROOT_DIR)/benchmark-seqio: seqio.c $(seqioObj)
	@$(cc) $(CFLAGS) -DREAD -o $(ROOT_DIR)/benchmark-seqio-read $^ $(LIBS)
	@$(cc) $(CFLAGS) -DWRITE -o $(ROOT_DIR)/benchmark-seqio-write $^ $(LIBS)
	@$(cc) $(CFLAGS) -DWRITE_GZ -o $(ROOT_DIR)/benchmark-seqio-write-gz $^ $(LIBS)

$(ROOT_DIR)/benchmark-kseq: kseq.c kseq.h $(seqioObj)
	@$(cc) $(CFLAGS) -o $@ $^ $(LIBS)
