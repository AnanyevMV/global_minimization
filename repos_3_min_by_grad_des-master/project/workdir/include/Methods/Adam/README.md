# Adam

Adaptive Moment Estimation (Adam) is another method that computes adaptive 
learning rates for each parameter. In addition to storing an exponentially 
decaying average of past squared gradients $`{v_t}`$ like Adadelta and RMSprop, 
Adam also keeps an exponentially decaying average of past gradients $`{m_t}`$, 
similar to momentum. Whereas momentum can be seen as a ball running down a slope, 
Adam behaves like a heavy ball with friction, which thus prefers flat minima in 
the error surface. We compute the decaying averages of past and past squared 
gradients $`{m_t}`$ and $`{v_t}`$ respectively as follows:

```math
m_t = {{\beta}_1}{m_{t-1}} + (1 - {\beta}_1){g_t}
```

```math
v_t = {{\beta}_2}{v_{t-1}} + (1 - {\beta}_2){g_t}^2
```

$`{m_t}`$ and $`{v_t}`$ are estimates of the first moment (the mean) and the 
second moment (the uncentered variance) of the gradients respectively, 
hence the name of the method. As $`{m_t}`$ and $`{v_t}`$ are initialized as 
vectors of 0's, the authors of Adam observe that they are biased towards zero, 
especially during the initial time steps, and especially when the decay rates 
are small (i.e. $`{\beta}_1`$ and $`{\beta}_2`$ are close to 1).

They counteract these biases by computing bias-corrected first and second moment 
estimates:

```math
\hat{m_t} = \frac {m_t} {1 - {{\beta}_1}^t}
```

```math
\hat{v_t} = \frac {v_t} {1 - {{\beta}_2}^t}
```

```math
{\theta}_{t+1} = {\theta}_t - \frac {\eta}{\sqrt{\hat{v_t}} + {\epsilon}} {\hat{m_t}}
```

The authors propose default values of $`0.9`$ for $`{\beta}_1`$ and $`0.999`$ for 
$`{\beta}_2`$, and $`{10^{-8}}`$ for $`{\epsilon}`$. They show empirically 
that Adam works well in practice and compares favorably to other adaptive 
learning-method algorithms.