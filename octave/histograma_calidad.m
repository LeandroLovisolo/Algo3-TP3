function histograma_calidad(nombre_algoritmo, data)

titulo = cstrcat('Heurística ', nombre_algoritmo, ': calidad de las soluciones');
xlabel = 'Tamaño de la frontera: diferencia respecto de la solución exacta';

histograma(data, titulo, xlabel);