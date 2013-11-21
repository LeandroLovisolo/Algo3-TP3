#!/usr/bin/env python2
# coding: utf-8

###############################################################################
# Tests PROVISORIOS para verificar que no se alteren los outputs de los       #
# algoritmos luego de hacer una modificación.                                 #
###############################################################################

import os
from subprocess import Popen, PIPE

binaries = ["exacto", "golosa", "local", "tabu"]

files = []

for file in os.listdir("data"):
    if file.endswith(".in"):
        files.append(file)

for file in files:
    for binary in binaries:
        cmd = "./" + binary + " < data/" + file
        print cmd

        process = Popen(cmd, stdout=PIPE, shell=True)
        (output, err) = process.communicate()
        exit_code = process.wait()

        # Este mismo archivo se usa tanto para guardar el output
        # de los algoritmos como para verificar que el output coincida
        # con lo guardado. Para guardar el output, descomentar el primer
        # bloque. Para verificar el output, descomentar el segundo.

        # # Guardar el output
        # f = open("out/" + binary + "_" + file, "w")
        # f.write(output)
        # f.close()

        # Verificar el output
        f = open("out/" + binary + "_" + file, "r")
        correct_output = f.read()
        f.close()
        if(output != correct_output):
            print "WRONG OUTPUT!"
            print "Was:"
            print output
            print "Should be:"
            print correct_output