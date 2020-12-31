import pandas as pd
import seaborn as sns
import os.path
import matplotlib.pyplot as plt

from IPython import get_ipython
get_ipython().run_line_magic('matplotlib', 'inline')

directory = os.path.dirname(os.path.abspath(__file__))
filename = "timings_single.csv"
file_path = os.path.join(directory, filename)

x = pd.read_csv(file_path)
#plt.figure(figsize=(25,10))
sns.set_style("darkgrid")

#sns.boxplot(x="Sample Size", y="Time", data=x, color="blue", orient="v")
sns.scatterplot(x="Sample Size", y="Time", data=x)
#sns.catplot(y="Sample Size", x="Time", data=x, color="blue", kind="box", orient="h")

plt.savefig("plot_timings" + ".png", dpi=100)
plt.show()

