# Adadelta

Adadelta is an extension of Adagrad that seeks to reduce its aggressive, 
monotonically decreasing learning rate. Instead of accumulating all past 
squared gradients, Adadelta restricts the window of accumulated past gradients 
to some fixed size $`w`$.

Instead of inefficiently storing $`w`$ previous squared gradients, 
the sum of gradients is recursively defined as a decaying average 
of all past squared gradients. The running average $`{E[g^2]_{t}}`$ at time step 
$`{t}`$ then depends (as a fraction $`\gamma`$ similarly to the Momentum term) 
only on the previous average and the current gradient:

```math
E[g^2]_t = {\gamma}E[g^2]_{t-1} + (1 - \gamma){g^2}_t
```

We set $`{\gamma}`$ to a similar value as the momentum term, around $`0.9`$. 
For clarity, we now rewrite our vanilla SGD update in terms of the parameter 
update vector $`{\bigtriangleup}{\theta}_{t}`$:

```math
{\bigtriangleup}{\theta}_{t} = -{\eta}*{g_{t,i}}
```

```math
{\theta}_{t+1} = {\theta}_{t} + {\bigtriangleup}{\theta}_{t}
```

The parameter update vector of Adagrad that we derived previously thus takes the form:

```math
{\bigtriangleup}{\theta}_{t} = - \frac {\eta}{\sqrt{G_t + \varepsilon}} * {g_t}
```