function histograma(data, titulo, eje_x)

hist(data, 50);
title(latex_escape(titulo));
xlabel(latex_escape(eje_x));
ylabel('Frecuencia');
