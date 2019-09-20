# RmsProp

RMSprop is an unpublished, adaptive learning rate method proposed by 
Geoff Hinton in Lecture 6e of his Coursera Class.

RMSprop and Adadelta have both been developed independently around the same time 
stemming from the need to resolve Adagrad's radically diminishing learning rates. 
RMSprop in fact is identical to the first update vector of Adadelta that we derived 
above:

```math
E[g^2]_t = 0.9{E[g^2]_{t-1}} + 0.1{g^2}_{t}
```

```math
{\theta}_{t+1} = {\theta}_{t} - \frac {\eta} {\sqrt{E[g^2]_{t} + {\varepsilon}}} * g_t
```

RMSprop as well divides the learning rate by an exponentially decaying average 
of squared gradients. Hinton suggests $`{\gamma}`$ to be set to $`{0.9}`$, 
while a good default value for the learning rate $`{\eta}`$ is $`{0.001}`$.