import pandas as pd
import seaborn as sns
import os.path
import matplotlib.pyplot as plt
import numpy as np
import math
import scipy.stats as stats
from IPython import get_ipython
from scipy.stats import norm
from labellines import labelLine, labelLines

get_ipython().run_line_magic('matplotlib', 'inline')

name = "8khz_9_mm_gunshot_mono_ext_E1_d"
sufix = "_wn"
ext = ".csv"
directory = os.path.dirname(os.path.abspath(__file__))
sz = 1500
k_len = 0

def power_fn(arr_tmp):
    sum = 0
    N = len(arr_tmp)
    global k_len

    for k in range(0, len(arr_tmp)-1):
        sum = sum + (pow(arr_tmp[k], 2) * (k + (k_len * N)))
                       
    
    sum = (1/N ) * sum
    k_len = k_len + 1
    return float(sum)

def mean_fn(arr_tmp):
    total = 0;
    for k in arr_tmp:
        total += k;
        
    return total/len(arr_tmp);

def std_fn(arr_tmp, media_tmp, power_len):
    std_local = 0;
    for k in range(0, len(arr_tmp)):
        std_local += pow(arr_tmp[k] - media_tmp, 2);
    return math.sqrt(std_local/(power_len-1))

def graph(df, name, sufix, i, x):
    arr = []
    power_arr = []
    global k_len
    k_len = 0
    pos = 0
    mean = 0
    std_var =0
    for index, row in df.iterrows():
        if(pos == sz):
            power_arr.append(power_fn(arr))
            arr = []
            pos = 0
            
        arr.append(float(row[0]))
        pos += 1
        
    power_arr.append(float(power_fn(arr)))
    mean = mean_fn(power_arr)
    std_var = std_fn(power_arr, mean, len(power_arr))
    th = (0.7 * std_var) + mean;
    print(th)
  
    th_arr = np.array(th)
    np_arr = np.array(power_arr)
    #df_arr = pd.DataFrame(data=np_arr.flatten())
    #df_arr.plot.hist()
    #df_arr.index.name = "sample"
    mu = 0
    std = x
    #title = "Histograma, distribuição dos valores: mu = %.2f,  std = %.2f" % (mu, std)
    plt.title("D" + str(i) + " White noise std: " + str(x))
    #sns.distplot(df_arr, bins=100, fit=norm, hist=True, kde=False)
   
    sns.lineplot(data=np_arr)
    
    plt.axhline(y=th, color='r', linestyle='-', label=" Threshold")
    plt.axhline(y=mean, color='g', linestyle='-', label="Mean")
    plt.xlabel('Block index k')
    plt.ylabel('Power')
    plt.legend()
    plt.gcf().subplots_adjust(left=0.15, bottom=0.15)
    #labelLines(plt.gca().get_lines(), xvals=(0.1, 1), zorder=2.5)
    if x == 0:
        plt.savefig(name + str(i) + ".png")
    elif x > 0:
        plt.savefig(name + str(i) + sufix + str(x) + ".png")
    plt.close()


for i in range(1, 5):
    
    name_orig = name + str(i)
    fp_orig = os.path.join(directory, name_orig+ext)
    orig = pd.read_csv(fp_orig)
    tmp = []
    for index, linha in orig.iterrows():
        tmp.append(float(linha[0]))
    graph(orig, name, sufix, i, 0) 
    for x in range(1 , 5):
        filename = name + str(i) + sufix + str(x) + ext
        file_path = os.path.join(directory, filename)
        wn = pd.read_csv(file_path)
        graph(wn, name, sufix, i, x)    


    