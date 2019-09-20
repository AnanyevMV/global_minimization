# Adagrad

Adagrad is an algorithm for gradient-based optimization that does just this: It 
adapts the learning rate to the parameters, performing smaller updates
(i.e. low learning rates) for parameters associated with frequently occurring 
features, and larger updates (i.e. high learning rates) for parameters associated 
with infrequent features. For this reason, it is well-suited for dealing with 
sparse data.

Previously, we performed an update for all parameters $`{\theta}`$ at once as every 
parameter $`{\theta}_{i}`$ used the same learning rate $`{\eta}`$. As Adagrad uses 
a different learning rate for every parameter $`{\theta}_{i}`$ at every time step 
$`t`$ we first show Adagrad's per-parameter update, which we then vectorize.
For brevity, we use $`{g}_{t}`$ to denote the gradient at time step $`t`$.
$`g_{t,i}`$ is then the partial derivative of the objective function w.r.t. to 
the parameter $` {\theta}_{i} `$
at time step
$`t`$:

```math
g_{t,i} = {\bigtriangleup}_{\theta}J(\theta_{t,i})
```

The SGD update for every $`{\theta}_{i}`$ at each time step $`{t}`$ then becomes:
```math
{\theta}_{t+1,i} = {\theta}_{t,i} - {\eta}*{g_{t,i}}
```

In its update rule, Adagrad modifies the general learning rate $`{\eta}`$ at 
each time step $`{t}`$ for every parameter $`{\theta}_{i}`$ based on 
the past gradients that have been computed for $`{\theta}_{i}`$:

```math
{\theta}_{t+1,i} = {\theta}_{t,i} - \frac {\eta}{\sqrt{G_{t,ii}+{\varepsilon}}}
```

