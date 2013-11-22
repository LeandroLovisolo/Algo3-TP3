function histograma_tiempo(nombre_algoritmo, data)

titulo = cstrcat('Heurística ', nombre_algoritmo, ': tiempo de ejecución');
xlabel = 'Tiempo de ejecución (milisegundos)';

histograma(data, titulo, xlabel);