import pandas as pd
import seaborn as sns
import os.path
import matplotlib.pyplot as plt
import numpy as np
import math
import scipy.stats as stats
from IPython import get_ipython
from scipy.stats import norm
get_ipython().run_line_magic('matplotlib', 'inline')

name = "8khz_9_mm_gunshot_mono_ext_E1_d"
sufix = "_wn"
ext = ".csv"
directory = os.path.dirname(os.path.abspath(__file__))
for i in range(1, 5):
    
    name_orig = name + str(i)
    fp_orig = os.path.join(directory, name_orig+ext)
    orig = pd.read_csv(fp_orig)
    tmp = []
    for index, linha in orig.iterrows():
        tmp.append(float(linha[0]))
        
    for x in range(1 , 5):
        filename = name + str(i) + sufix + str(x) + ext
        file_path = os.path.join(directory, filename)
        wn = pd.read_csv(file_path)
        arr = []
        pos = 0
        for index, row in wn.iterrows():
            #print(float(row[0]) )
            arr.append(float(row[0]) - float(tmp[pos]))
            pos += 1
            
        np_arr = np.array(arr)
        df_arr = pd.DataFrame(data=np_arr.flatten())
        #df_arr.plot.hist()
        mu = 0
        std = x
        title = "White noise frequency. mu = %.2f,  std = %.2f" % (mu, std)
        plt.title(title)
        p = sns.distplot(df_arr, bins=200, fit=norm, hist=True, 
                 fit_kws ={"alpha": 1, "label":"Gauss distribution"}, 
                     hist_kws={"alpha": 1, "color":"#51b0f5", "label":"Sample obtained", "linewidth":"0.1"},
                     kde=False).set(xlim=(-15,15), ylim=(0, 0.5))
        plt.xlabel('Obtained value')
        plt.ylabel('Frequency')
        plt.legend()
       
        plt.savefig("D" + str(i) + "_std " + str(x) + ".png", dpi=100)
        plt.close()

#x = pd.read_csv(file_path)
#sns.set_style(darkgrid)
#plt.rcParams['figure.figsize'] = (12,24)
#print(x.head())
#sns.boxplot(x=Sample Size, y=Time, data=x, color=blue, orient=v)
#sns.catplot(y=Sample Size, x=Time, data=x, color=blue, kind=box, orient=h)


    