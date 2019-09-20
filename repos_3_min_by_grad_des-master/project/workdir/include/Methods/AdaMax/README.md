# AdaMax

The $`{v_t}`$ factor in the Adam update rule scales the gradient inversely 
proportionally to the $`{l_2}`$ norm of the past gradients (via the $`{v_{t-1}}`$ 
term) and current gradient $`{|g_t|^2}`$:

```math
v_t = {{\beta}_2}{v_{t-1}} + (1 - {\beta}_2){|g_t|}^2
```

We can generalize this update to the $`{l_p}`$ norm. 
Note that Kingma and Ba also parameterize $`{\beta}_2`$ as $`{{\beta}_2}^p`$:

```math
v_t = {{\beta}_2}^p{v_{t-1}} + (1 - {{\beta}_2}^p){|g_t|}^p
```

Norms for large $`{p}`$ values generally become numerically unstable, which is 
why $`{l_1}`$ and $`{l_1}`$ norms are most common in practice. However, $`{l_{\infty}}`$
also generally exhibits stable behavior. For this reason, the authors propose 
AdaMax (Kingma and Ba, 2015) and show that $`{v_t}`$ with $`{l_{\infty}}`$ converges 
to the following more stable value. To avoid confusion with Adam, we use $`u_t`$ 
to denote the infinity norm-constrained $`{v_t}`$:

```math
u_t = {{\beta}_2}^{\infty} {v_{t-1}} + (1 - {{\beta}_2}^{\infty}) {|g_t|}^{\infty} = max({\beta}_2 * v_{t-1}, |g_t|)
```

We can now plug this into the Adam update equation by replacing 
$`{\sqrt{\hat{v_t}}} + \epsilon`$ with $`{u_t}`$ to obtain the AdaMax update rule:

```math
{\theta}_{t+1} = {\theta}_{t} - \frac {\eta} {u_t} {\hat{m_t}}
```

Note that as $`u_t`$ relies on the $`max`$ operation, it is not as 
suggestible to bias towards zero as $`m_t`$ and $`v_t`$ in Adam, which is why 
we do not need to compute a bias correction for $`{u_t}`$. Good default values 
are again $`{\eta = 0.002}`$, $`\beta_1 = 0.9`$ and $`\beta_2 = 0.999`$.
