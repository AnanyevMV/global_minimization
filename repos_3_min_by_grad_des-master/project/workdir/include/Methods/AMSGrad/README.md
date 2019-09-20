# AMSGrad

As adaptive learning rate methods have become the norm in training neural networks, 
practitioners noticed that in some cases, e.g. for object recognition or machine 
translation they fail to converge to an optimal solution and are outperformed by 
SGD with momentum.

Reddi et al. (2018) formalize this issue and pinpoint the exponential moving 
average of past squared gradients as a reason for the poor generalization 
behaviour of adaptive learning rate methods. Recall that the introduction of 
the exponential average was well-motivated: It should prevent the learning rates 
to become infinitesimally small as training progresses, the key flaw of the Adagrad 
algorithm. However, this short-term memory of the gradients becomes an obstacle in
other scenarios.

In settings where Adam converges to a suboptimal solution, it has been observed 
that some minibatches provide large and informative gradients, but as these 
minibatches only occur rarely, exponential averaging diminishes their influence, 
which leads to poor convergence. The authors provide an example for a simple convex 
optimization problem where the same behaviour can be observed for Adam.

To fix this behaviour, the authors propose a new algorithm, 
AMSGrad that uses the maximum of past squared gradients $`{v_t}`$ rather than the 
exponential average to update the parameters. 
$`{v_t}`$ is defined the same as in Adam above:

```math
v_t = {\beta}_2 {v_{t-1}} + (1 - {\beta}_2){g_t}^2
```

Instead of using $`{v_t}`$ (or its bias-corrected version $`\hat{v_t}`$) directly, 
we now employ the previous $`{v_{t-1}}`$ if it is larger than the current one:

```math
\hat{v_t} = max(\hat{v_{t-1}},v_t)
```

This way, AMSGrad results in a non-increasing step size, which avoids the problems 
suffered by Adam. For simplicity, the authors also remove the debiasing step that 
we have seen in Adam. The full AMSGrad update without bias-corrected estimates can 
be seen below:

```math
m_t = {\beta}_1 {m_{t-1}} + (1 - {\beta}_1){g_t}
```

```math
v_t = {\beta}_2 {v_{t-1}} + (1 - {\beta}_2){g_t}^2
```

```math
\hat{v_t} = max(\hat{v_{t-1}},v_t)
```

```math
{\theta}_{t+1} = {\theta}_t - \frac {\eta} {\sqrt{\hat{v_t}} + \epsilon} {m_t}
```