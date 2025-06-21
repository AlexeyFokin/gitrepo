import pandas as pd

df = pd.read_csv(fr'c:\Users\lisak\Downloads\0\survey_results_public.csv')

#print(df['Country'].value_counts(normalize=True))

g = df.groupby(['Country'])

g.get_group([])