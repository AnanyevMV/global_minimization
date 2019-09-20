# Nadam

As we have seen before, Adam can be viewed as a combination of RMSprop and momentum: 
RMSprop contributes the exponentially decaying average of past squared gradients 
$`{v_t}`$, while momentum accounts for the exponentially decaying average of past 
gradients $`{m_t}`$. We have also seen that Nesterov accelerated gradient (NAG) 
is superior to vanilla momentum.

Nadam (Nesterov-accelerated Adaptive Moment Estimation) thus combines Adam and NAG. 
In order to incorporate NAG into Adam, we need to modify its momentum term $`{m_t}`$.

Nadam update rule:

```math
{\theta}_{t+1} = {\theta}_{t} - \frac {\eta} {\sqrt{\hat{v_t}} + \epsilon} ({\beta}_1 {\hat{m_t}+  {\frac {(1-{\beta}_1){g_t}} {1 -{{\beta}_1}^{t}}} )}
```