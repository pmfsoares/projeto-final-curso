import csv, sys, numpy, os
from pathlib import Path

def rnd_white_noise(arr, var, name):
    mean = 0
    std = var
    num_samples = len(arr)
    print(num_samples)
    samples = numpy.random.normal(mean, std, size=num_samples)
    new_vals = []
    for  i, ele in enumerate(arr):
        tmp = float(ele) + samples[i]
        new_vals.append(tmp)
    with open(name + "_wn" + str(var) + "_" + str(num_samples) + ".csv" , 'w', newline='\n') as csvfile:
        csvwrite = csv.writer(csvfile)
        name_arr = []
        name_arr.append(name + "_wn" + str(var) )
        name_arr.append("")
        csvwrite.writerow(name_arr)
        for val in new_vals:
            t = [ val , ""]
            csvwrite.writerow(t)

def extractCSV(filename):
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter = ',')
        line_count = 0
        name = ""
        csv_vals = []
        for row in csv_reader:
            if line_count == 0:
                name = row[0]
                line_count += 1
            else:
                mean=0
                csv_vals.append(row[0])
                #for x in range(1, 5):
                #    rnd_white_noise(row, x, name)
        for x in range(1, 5):
            rnd_white_noise(csv_vals, x, name)
for x in range(1, 5):
    s =  '8khz_9_mm_gunshot_mono_ext_E1_d'
    suffix = '.csv'
    extractCSV("./" + s + str(x) + "_15993" +  suffix)
