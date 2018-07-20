## Negative inversion

The film characterisic transfer curve $T$ gives the negative density as a
function of the scene radiance $R$. This curve is well approximated by:

$$ d = \gamma_F log(R) $$

The density measure corresponds to the log-ratio between some reference signal
level $I^\star$ and the scanned pixel's value $I$. High densities map to
low exposure in the scanned image, so we have:

$$ d = -\log\left(\frac{I}{I^\star}\right) $$

We want to recover the linear scene radiance signal $R$, which we compute as:

$$ R = \left(\frac{I^\star}{I}\right)^{\frac{1}{\gamma_F}} $$

Or for numerical stability:

$$ R = \left(\frac{1}{\epsilon + \frac{I}{I^\star}}\right)^{\frac{1}{\gamma_F}} $$

The division by $I^\star$ should factor a white-balance correction to
neutralize the light source that illuminates the negative and the film base.

