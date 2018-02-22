import pickle
f = open("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.pkl")
data = pickle.load(f)
for key, val in data:
    # variant 1)
    d1[key] = d1.get(key, []) + [val]
#dict.keys()
#print data
