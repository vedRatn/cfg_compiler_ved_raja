  Procedure: main
  Intermediate Code Statements

label2: 
 iLoad:	v0 <- 1
 store:	i <- v0
 iLoad:	v0 <- 2
 store:	j <- v0
 iLoad:	v0 <- 2
 store:	k <- v0
 iLoad:	v0 <- 2
 store:	l <- v0
 load:	v0 <- i
 store:	a <- v0
 load:	v0 <- i
 load:	t0 <- a
 slt: t1 <- v0 , t0
 load:	v0 <- j
 slt: t0 <- t1 , v0
 load:	v0 <- a
 sge: t1 <- t0 , v0
 load:	v0 <- k
 sne: t0 <- t1 , v0
 load:	v0 <- i
 seq: t1 <- t0 , v0
 bne: t1 , zero : goto label3
 goto label5

label3: 
 load:	v0 <- i
 iLoad:	t0 <- 1
 seq: t1 <- v0 , t0
 bne: t1 , zero : goto label4
 goto label5

label4: 
 iLoad:	v0 <- 3
 store:	j <- v0
 goto label9

label5: 
 goto label7

label6: 
 iLoad:	v0 <- 4
 store:	j <- v0

label7: 
 load:	v0 <- i
 load:	t0 <- j
 sgt: t1 <- v0 , t0
 bne: t1 , zero : goto label6
 goto label8

label8: 
 load:	v0 <- i
 iLoad:	t0 <- 1
 sne: t1 <- v0 , t0
 bne: t1 , zero : goto label6
 goto label9

label9: 
