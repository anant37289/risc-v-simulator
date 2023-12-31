# risc v simulater with intgrated cache
steps to run code:
- add the code t0 run in editor.txt 
- run assemble.cpp
- the assembled code gets stored in machinecode.txt
- run simulator.cpp

### Instructions supported:
instruction-->format
```
add     add x1,x2,x3
sub     sub x1,x2,x3
and     and x1,x2,x3
or      or x1,x2,x3
addi    addi x1,x2,1
lw      lw x1,x4,4
sw      sw x2,4,x3
beq     beq x1,x2,label
bge     blt x1,x2,label
blt     bge x1,x2,label
labels for branch also supported
comments are not supported
```
### code tested on
- prime
```
addi s1,x0,7
addi t0,x0,2
loop:
bge t0,s1,prime
add t1,s1,x0
continued_sub:
sub t1,t1,t0
blt x0,t1,continued_sub
beq t1,x0,not_prime
addi t0,t0,1
beq x0,x0,loop
prime:
addi s2,x0,1
beq x0,x0,not_prime
not_prime:
```
- gcd and lcm
```
#code for gcd lcm
addi x1,x0,8
addi x2,x0,2
blt x1,x2,x1_lt_x2
add s1,x1,x0
add s2,x2,x0
beq x0,x0,compare_exit
x1_lt_x2:
add s1,x2,x0
add s2,x1,x0
compare_exit:
add t0,s1,x0
add t1,s2,x0
gcd_loop:
sub t0,t0,t1
blt t1,t0,gcd_loop
beq t0,x0,gcd_exit
add t2,t1,x0
add t1,t0,x0
add t0,t2,x0
beq x0,x0,gcd_loop
gcd_exit:
add t2,x0,x0
loop:
add t2,t2,x1
addi x2,x2,-1
blt x0,x2,loop
addi t3,x0,1
loop2:
sub t2,t2,t1
addi t3,t3,1
blt t1,t2,loop2
add x0,x0,x0
```
- factorial
```
addi s1,x0,1
addi s2,x0,7
addi x3,x0,2
fact_loop:
add x2,x3,x0
loop:
add t0,t0,s1
addi x2,x2,-1
blt x0,x2,loop
addi x3,x3,1
add s1,t0,x0
add t0,x0,x0
bge s2,x3,fact_loop
```
### results from sample run
```
cycle fetch   decode  execute memory  writeback
1       0       -2      -2      -2      -2
2       4       0       -2      -2      -2
3       8       4       0       -2      -2
4       12      8       4       0       -2
5       16      12      8       4       0
6       20      16      12      8       4
7       24      20      16      12      8
8       28      24      20      16      12
9       32      28      24      20      16
10      36      32      28      24      20
11      40      36      32      28      24
12      24      -1      -1      32      28
13      28      24      -1      -1      32
14      32      28      24      -1      -1
15      36      32      28      24      -1
16      40      36      32      28      24
17      44      40      36      32      28
18      48      44      40      36      32
19      52      48      44      40      36
20      -2      52      48      44      40
21      20      -1      -1      48      44
22      24      20      -1      -1      48
23      28      24      20      -1      -1
24      32      28      24      20      -1
25      36      32      28      24      20
26      40      36      32      28      24
27      24      -1      -1      32      28
28      28      24      -1      -1      32
29      32      28      24      -1      -1
30      36      32      28      24      -1
31      40      36      32      28      24
32      24      -1      -1      32      28
33      28      24      -1      -1      32
34      32      28      24      -1      -1
35      36      32      28      24      -1
36      40      36      32      28      24
37      44      40      36      32      28
38      48      44      40      36      32
39      52      48      44      40      36
40      -2      52      48      44      40
41      20      -1      -1      48      44
42      24      20      -1      -1      48
43      28      24      20      -1      -1
44      32      28      24      20      -1
45      36      32      28      24      20
46      40      36      32      28      24
47      24      -1      -1      32      28
48      28      24      -1      -1      32
49      32      28      24      -1      -1
50      36      32      28      24      -1
51      40      36      32      28      24
52      24      -1      -1      32      28
53      28      24      -1      -1      32
54      32      28      24      -1      -1
55      36      32      28      24      -1
56      40      36      32      28      24
57      24      -1      -1      32      28
58      28      24      -1      -1      32
59      32      28      24      -1      -1
60      36      32      28      24      -1
61      40      36      32      28      24
62      44      40      36      32      28
63      48      44      40      36      32
64      52      48      44      40      36
65      -2      52      48      44      40
66      20      -1      -1      48      44
67      24      20      -1      -1      48
68      28      24      20      -1      -1
69      32      28      24      20      -1
70      36      32      28      24      20
71      40      36      32      28      24
72      24      -1      -1      32      28
73      28      24      -1      -1      32
74      32      28      24      -1      -1
75      36      32      28      24      -1
76      40      36      32      28      24
77      24      -1      -1      32      28
78      28      24      -1      -1      32
79      32      28      24      -1      -1
80      36      32      28      24      -1
81      40      36      32      28      24
82      24      -1      -1      32      28
83      28      24      -1      -1      32
84      32      28      24      -1      -1
85      36      32      28      24      -1
86      40      36      32      28      24
87      24      -1      -1      32      28
88      28      24      -1      -1      32
89      32      28      24      -1      -1
90      36      32      28      24      -1
91      40      36      32      28      24
92      44      40      36      32      28
93      48      44      40      36      32
94      52      48      44      40      36
95      -2      52      48      44      40
96      20      -1      -1      48      44
97      24      20      -1      -1      48
98      28      24      20      -1      -1
99      32      28      24      20      -1
100     36      32      28      24      20
101     40      36      32      28      24
102     24      -1      -1      32      28
103     28      24      -1      -1      32
104     32      28      24      -1      -1
105     36      32      28      24      -1
106     40      36      32      28      24
107     24      -1      -1      32      28
108     28      24      -1      -1      32
109     32      28      24      -1      -1
110     36      32      28      24      -1
111     40      36      32      28      24
112     24      -1      -1      32      28
113     28      24      -1      -1      32
114     32      28      24      -1      -1
115     36      32      28      24      -1
116     40      36      32      28      24
117     24      -1      -1      32      28
118     28      24      -1      -1      32
119     32      28      24      -1      -1
120     36      32      28      24      -1
121     40      36      32      28      24
122     24      -1      -1      32      28
123     28      24      -1      -1      32
124     32      28      24      -1      -1
125     36      32      28      24      -1
126     40      36      32      28      24
127     44      40      36      32      28
128     48      44      40      36      32
129     52      48      44      40      36
130     -2      52      48      44      40
131     20      -1      -1      48      44
132     24      20      -1      -1      48
133     28      24      20      -1      -1
134     32      28      24      20      -1
135     36      32      28      24      20
136     40      36      32      28      24
137     24      -1      -1      32      28
138     28      24      -1      -1      32
139     32      28      24      -1      -1
140     36      32      28      24      -1
141     40      36      32      28      24
142     24      -1      -1      32      28
143     28      24      -1      -1      32
144     32      28      24      -1      -1
145     36      32      28      24      -1
146     40      36      32      28      24
147     24      -1      -1      32      28
148     28      24      -1      -1      32
149     32      28      24      -1      -1
150     36      32      28      24      -1
151     40      36      32      28      24
152     24      -1      -1      32      28
153     28      24      -1      -1      32
154     32      28      24      -1      -1
155     36      32      28      24      -1
156     40      36      32      28      24
157     24      -1      -1      32      28
158     28      24      -1      -1      32
159     32      28      24      -1      -1
160     36      32      28      24      -1
161     40      36      32      28      24
162     24      -1      -1      32      28
163     28      24      -1      -1      32
164     32      28      24      -1      -1
165     36      32      28      24      -1
166     40      36      32      28      24
167     44      40      36      32      28
168     48      44      40      36      32
169     52      48      44      40      36
170     -2      52      48      44      40
171     20      -1      -1      48      44
172     24      20      -1      -1      48
173     28      24      20      -1      -1
174     32      28      24      20      -1
175     36      32      28      24      20
176     40      36      32      28      24
177     24      -1      -1      32      28
178     28      24      -1      -1      32
179     32      28      24      -1      -1
180     36      32      28      24      -1
181     40      36      32      28      24
182     24      -1      -1      32      28
183     28      24      -1      -1      32
184     32      28      24      -1      -1
185     36      32      28      24      -1
186     40      36      32      28      24
187     24      -1      -1      32      28
188     28      24      -1      -1      32
189     32      28      24      -1      -1
190     36      32      28      24      -1
191     40      36      32      28      24
192     24      -1      -1      32      28
193     28      24      -1      -1      32
194     32      28      24      -1      -1
195     36      32      28      24      -1
196     40      36      32      28      24
197     24      -1      -1      32      28
198     28      24      -1      -1      32
199     32      28      24      -1      -1
200     36      32      28      24      -1
201     40      36      32      28      24
202     24      -1      -1      32      28
203     28      24      -1      -1      32
204     32      28      24      -1      -1
205     36      32      28      24      -1
206     40      36      32      28      24
207     24      -1      -1      32      28
208     28      24      -1      -1      32
209     32      28      24      -1      -1
210     36      32      28      24      -1
211     40      36      32      28      24
212     44      40      36      32      28
213     48      44      40      36      32
214     52      48      44      40      36
215     -2      52      48      44      40
216     20      -1      -1      48      44
217     24      20      -1      -1      48
218     28      24      20      -1      -1
219     32      28      24      20      -1
220     36      32      28      24      20
221     40      36      32      28      24
222     24      -1      -1      32      28
223     28      24      -1      -1      32
224     32      28      24      -1      -1
225     36      32      28      24      -1
226     40      36      32      28      24
227     24      -1      -1      32      28
228     28      24      -1      -1      32
229     32      28      24      -1      -1
230     36      32      28      24      -1
231     40      36      32      28      24
232     24      -1      -1      32      28
233     28      24      -1      -1      32
234     32      28      24      -1      -1
235     36      32      28      24      -1
236     40      36      32      28      24
237     24      -1      -1      32      28
238     28      24      -1      -1      32
239     32      28      24      -1      -1
240     36      32      28      24      -1
241     40      36      32      28      24
242     24      -1      -1      32      28
243     28      24      -1      -1      32
244     32      28      24      -1      -1
245     36      32      28      24      -1
246     40      36      32      28      24
247     24      -1      -1      32      28
248     28      24      -1      -1      32
249     32      28      24      -1      -1
250     36      32      28      24      -1
251     40      36      32      28      24
252     24      -1      -1      32      28
253     28      24      -1      -1      32
254     32      28      24      -1      -1
255     36      32      28      24      -1
256     40      36      32      28      24
257     24      -1      -1      32      28
258     28      24      -1      -1      32
259     32      28      24      -1      -1
260     36      32      28      24      -1
261     40      36      32      28      24
262     44      40      36      32      28
263     48      44      40      36      32
264     52      48      44      40      36
265     -2      52      48      44      40
266     20      -1      -1      48      44
267     24      20      -1      -1      48
268     28      24      20      -1      -1
269     32      28      24      20      -1
270     36      32      28      24      20
271     40      36      32      28      24
272     24      -1      -1      32      28
273     28      24      -1      -1      32
274     32      28      24      -1      -1
275     36      32      28      24      -1
276     40      36      32      28      24
277     24      -1      -1      32      28
278     28      24      -1      -1      32
279     32      28      24      -1      -1
280     36      32      28      24      -1
281     40      36      32      28      24
282     24      -1      -1      32      28
283     28      24      -1      -1      32
284     32      28      24      -1      -1
285     36      32      28      24      -1
286     40      36      32      28      24
287     24      -1      -1      32      28
288     28      24      -1      -1      32
289     32      28      24      -1      -1
290     36      32      28      24      -1
291     40      36      32      28      24
292     24      -1      -1      32      28
293     28      24      -1      -1      32
294     32      28      24      -1      -1
295     36      32      28      24      -1
296     40      36      32      28      24
297     24      -1      -1      32      28
298     28      24      -1      -1      32
299     32      28      24      -1      -1
300     36      32      28      24      -1
301     40      36      32      28      24
302     24      -1      -1      32      28
303     28      24      -1      -1      32
304     32      28      24      -1      -1
305     36      32      28      24      -1
306     40      36      32      28      24
307     24      -1      -1      32      28
308     28      24      -1      -1      32
309     32      28      24      -1      -1
310     36      32      28      24      -1
311     40      36      32      28      24
312     24      -1      -1      32      28
313     28      24      -1      -1      32
314     32      28      24      -1      -1
315     36      32      28      24      -1
316     40      36      32      28      24
317     44      40      36      32      28
318     48      44      40      36      32
319     52      48      44      40      36
320     -2      52      48      44      40
321     20      -1      -1      48      44
322     24      20      -1      -1      48
323     28      24      20      -1      -1
324     32      28      24      20      -1
325     36      32      28      24      20
326     40      36      32      28      24
327     24      -1      -1      32      28
328     28      24      -1      -1      32
329     32      28      24      -1      -1
330     36      32      28      24      -1
331     40      36      32      28      24
332     24      -1      -1      32      28
333     28      24      -1      -1      32
334     32      28      24      -1      -1
335     36      32      28      24      -1
336     40      36      32      28      24
337     24      -1      -1      32      28
338     28      24      -1      -1      32
339     32      28      24      -1      -1
340     36      32      28      24      -1
341     40      36      32      28      24
342     24      -1      -1      32      28
343     28      24      -1      -1      32
344     32      28      24      -1      -1
345     36      32      28      24      -1
346     40      36      32      28      24
347     24      -1      -1      32      28
348     28      24      -1      -1      32
349     32      28      24      -1      -1
350     36      32      28      24      -1
351     40      36      32      28      24
352     24      -1      -1      32      28
353     28      24      -1      -1      32
354     32      28      24      -1      -1
355     36      32      28      24      -1
356     40      36      32      28      24
357     24      -1      -1      32      28
358     28      24      -1      -1      32
359     32      28      24      -1      -1
360     36      32      28      24      -1
361     40      36      32      28      24
362     24      -1      -1      32      28
363     28      24      -1      -1      32
364     32      28      24      -1      -1
365     36      32      28      24      -1
366     40      36      32      28      24
367     24      -1      -1      32      28
368     28      24      -1      -1      32
369     32      28      24      -1      -1
370     36      32      28      24      -1
371     40      36      32      28      24
372     24      -1      -1      32      28
373     28      24      -1      -1      32
374     32      28      24      -1      -1
375     36      32      28      24      -1
376     40      36      32      28      24
377     44      40      36      32      28
378     48      44      40      36      32
379     52      48      44      40      36
380     -2      52      48      44      40
381     -2      -2      52      48      44
382     -2      -2      -2      52      48
383     -2      -2      -2      -2      52
GPR[0]: 0
GPR[1]: 0
GPR[2]: 0
GPR[3]: 12
GPR[4]: 0
GPR[5]: 0
GPR[6]: 0
GPR[7]: 0
GPR[8]: 0
GPR[9]: 39916800
GPR[10]: 0
GPR[11]: 0
GPR[12]: 0
GPR[13]: 0
GPR[14]: 0
GPR[15]: 0
GPR[16]: 0
GPR[17]: 0
GPR[18]: 11
GPR[19]: 0
GPR[20]: 0
GPR[21]: 0
GPR[22]: 0
GPR[23]: 0
GPR[24]: 0
GPR[25]: 0
GPR[26]: 0
GPR[27]: 0
GPR[28]: 0
GPR[29]: 0
GPR[30]: 0
GPR[31]: 0
```
- the code stores 11! in s1/x9