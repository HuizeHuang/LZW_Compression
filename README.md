# LZW_Compression
Develop a compressor that can receive data in real time at modern ethernet speeds (1Gb/s,
maybe 10Gb/s) and compress it into memory using deduplication and compression.  Specif-
ically,  we’ll  look  at  Content-Defined  Chunking  to  break  the  input  into  chunks,  SHA-256
hashes  to  screen  for  duplicate  chunks,  and  LZW  compression  to  compress  non-duplicate
chunks.  Your goal for implementation is to achieve real-time guaranteed support of 1Gb/s
input stream,  but you may need to consider intermediate goals (e.g.  100Mb/s,  300Mb/s)
along the way.  In any case, you will sketch the solution to scale to 10Gb/s even if you cannot
implement it.
![project](https://user-images.githubusercontent.com/42952482/51547594-a2cd3180-1e34-11e9-9454-4e20eafc4a61.png)
