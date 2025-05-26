# Práctica 5. Clara Sola Ruiz

library(tuneR)
library(seewave)
library(audio)

# --------- Paso 1: Cargar los sonidos grabados (usando una grabadora online) ---------
nombre <- readWave("nombre.wav")
apellido <- readWave("apellido.wav")

# --------- Paso 2: Dibujar forma de onda ---------
# Guardar las formas de onda en un archivo PNG
png("formas_de_onda.png", width = 800, height = 600)

# Dibujar las formas de onda en el archivo
par(mfrow=c(2,1))
plot(nombre, main = "Onda: Nombre")
plot(apellido, main = "Onda: Apellido")

# Cerrar el archivo
dev.off()

# --------- Paso 3: Cabeceras ---------
str(nombre)
str(apellido)

# --------- Paso 4: Unir sonidos ---------
completo <- bind(nombre, apellido)

# --------- Paso 6: Guardar sonido ---------
writeWave(completo, "basico.wav")

# --------- Paso 5: Dibujar forma de onda y escuchar el audio ---------
png("onda_nombre_apellido", width = 800, height = 600)
plot(completo, main = "Onda: Nombre y Apellido")
dev.off()

system("aplay basico.wav")  

# --------- Paso 7: Filtro (eliminar entre 10kHz y 20kHz) ---------
filtrado <- bwfilter(completo, f = completo@samp.rate, n = 1, from = 10000, to = 20000, bandpass = FALSE, listen = TRUE)
filtrado_wave <- Wave(left = filtrado, samp.rate = completo@samp.rate, bit = 16)
writeWave(filtrado_wave, "filtrado.wav")

png("filtrado.png", width = 800, height = 600)
plot(filtrado_wave, main = "Onda: Filtrado")
dev.off()

png("espectrograma_original.png", width = 800, height = 600)
spectro(completo, f = completo@samp.rate, main = "Original")
dev.off()

png("espectograma_filtrado.png", width = 800, height = 600)
spectro(filtrado, f = completo@samp.rate, main = "Filtrado")
dev.off()

# --------- Paso 8: Eco + reverso ---------

# Aplicar eco
eco_wave <- echo(
  completo, 
  f = completo@samp.rate, 
  amp = c(0.8, 0.4, 0.2), 
  delay = c(0.3, 0.6, 0.9), 
  output = "Wave"
)

eco_wave@left <- 10000 * eco_wave@left

# Guardar el resultado con eco
writeWave(eco_wave, "eco.wav")
system("aplay eco.wav")

png("eco.png", width = 800, height = 600)
plot(eco_wave, main = "Onda: Eco")
dev.off()

# Reverso del audio con eco
alreves <- revw(eco_wave, output = "Wave")

# Guardar el audio al revés
writeWave(alreves, "alreves.wav")
system("aplay alreves.wav")

png("onda_alreves.png", width = 800, height = 600)
plot(alreves, main = "Onda: Audio al revés")
dev.off()