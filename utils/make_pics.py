
# coding: utf-8

# In[1]:

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd

import os


# In[2]:

pics_dir = os.path.join('paper', 'pics')


# In[3]:

data_path = os.path.join('bench-out', '1200x1800.csv')


# In[4]:

data = pd.read_csv(data_path)[['counter', 'grid', 'density', 'cpu time ms']].fillna('')


# In[5]:

COUNTER_MAP = {
    "DecisionTreeDsuCounter": "DTSUF",
    "DfsCounter": "DFS",
    "DsuCounter": "SUF",
    "TwolineDsuCounter": "SUF2"
}
data['counter'] = data['counter'].apply(lambda c: COUNTER_MAP.get(c) or c)


# In[6]:

GRID_MAP = {
    'SimpleGrid': '',
    'Viewing2x2Grid': ' on 2x2 view',
    'Compressing2x2Grid': ' on 2x2 copy'
}
data['grid'] = data['grid'].apply(lambda c: GRID_MAP.get(c) or c)


# In[7]:

data['cg'] = data['counter'] + data['grid']


# In[8]:

piv = data.pivot(index='cg', columns='density', values='cpu time ms').swapaxes(0, 1)
piv.head(2)


# In[9]:

cg = [(cou, gri) 
      for cou in ('DFS', 'SUF', 'DTSUF', 'SUF2')
      for gri in ('', ' on 2x2 view', ' on 2x2 copy')
      if (cou + gri in piv.columns)]


# In[10]:

METHOD_COLOR = {
    'DFS': 'red',
    'SUF': 'green',
    "SUF2": 'blue',
    'DTSUF': 'purple'
}

GRID_SHAPE = {
    '': '-',
    ' on 2x2 view': '--',
    ' on 2x2 copy': '-.'
}


# In[67]:

def add_plot(df, counter, grid, legend_prefix=''):
    name = counter + grid
    if name in df.columns:
        df[name].plot(
                style=GRID_SHAPE[grid],
                color=METHOD_COLOR[counter],
                label=legend_prefix + name)

def plot_all(df, font_size=None, ylabel=None, save=None, dpi=300, save_format='eps',
            legend_prefix='',
            legend_on_right=False, legend_loc=None):

    from matplotlib.font_manager import FontProperties

    fontP = FontProperties()
    if font_size:
        fontP.set_size(font_size)

    for counter, grid in cg:
        add_plot(df, counter=counter, grid=grid, legend_prefix=legend_prefix)

    plt.grid(True)
    if legend_on_right:
        lgd = plt.legend(loc=2, bbox_to_anchor=(1.01, 1), prop=fontP)
    else:
        lgd = plt.legend(loc=legend_loc, prop=fontP)

    plt.xlabel('density, %')
    if ylabel:
        plt.ylabel(ylabel)

    if save:
        plt.savefig(os.path.join(pics_dir, save + '.' + save_format),
            bbox_extra_artists=(lgd,), bbox_inches='tight',
            format=save_format, dpi=dpi)


# In[68]:

plot_all(piv, ylabel='cpu time, ms', save='overall',
        font_size='small', legend_loc=2)
plt.clf()


# In[69]:

def norm_by(df, cg):
    normed = df.copy()
    for column in normed.columns:
        normed[column] = df[cg] / df[column]
    return normed


# In[78]:

mpl.rcParams['axes.labelsize'] = 'xx-large'

for cou in ('DFS', 'SUF', 'SUF2', 'DTSUF'):
    plot_all(
        norm_by(piv, cou)[[c + g for c, g in cg if c == cou or {c, cou} == {'SUF', 'DTSUF'}]],
        save='normed_' + cou,
        ylabel='$T_{' + cou +  '}/T_A$',
        legend_prefix='A = ',
        legend_loc=1 if cou == 'DTSUF' else 2,
        font_size='x-large')
    plt.clf()


# In[ ]:



