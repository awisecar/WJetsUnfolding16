#! /usr/bin/env python3
import os
import sys
import numpy as np
import pandas as pd
import math
from array import array
import ROOT
#############################################

cwd = os.getcwd()
outputDir = "systFiles/"
os.system("mkdir -p "+outputDir)

#################
#doRatio = True
doRatio = False

year = "2016"
#################

if (doRatio):
    ### For Ratios
    unfoldDir = "UnfoldingCheck_Ratio/"
    variables = ["LeadingJetPt_2_Zinc2jet_LeadingJetPt_2_Zinc1jet"]
else:
    ### For Xsecs
    unfoldDir = "UnfoldingCheck_"+year+"/"
#    variables = ["FirstJetPt_Zinc1jet_TUnfold",
#                 "LepPtPlusLeadingJetPt_Zinc1jet_TUnfold", "LepPtPlusLeadingJetPt_Zinc2jet_TUnfold", "LepPtPlusLeadingJetPt_Zinc3jet_TUnfold",
#                 "LepPtPlusLeadingJetPt_Zexc1jet_TUnfold", "LepPtPlusLeadingJetPt_Zexc2jet_TUnfold", "LepPtPlusLeadingJetPt_Zexc3jet_TUnfold",
#                 "LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold", "LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold",
#                 "LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold", "LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold"
#                ]

variables = ["LepPtPlusLeadingJetPt_Zinc1jet_TUnfold"]

#    variables = ["dPhiLepJet1_Zinc1jet_TUnfold", "FirstJetAbsRapidity_Zinc1jet_TUnfold"
#                ]

# full syst
#names = ["Central", "PUUp", "PUDown", "JESUp", "JESDown", "XSECUp", "XSECDown", "JERUp", "JERDown", "SFUp", "SFDown", "BtagSFUp", "BtagSFDown", "LumiUp", "LumiDown"]
#namesLegend = ["Pileup", "Jet Energy Scale", "BG Cross Section", "Jet Energy Resolution", "Muon SFs", "B-Tag SFs", "Luminosity"]

# Central, JES, BG Xsec
names = ["Central", "JESUp", "JESDown", "XSECUp", "XSECDown"]
namesLegend = ["Jet Energy Scale", "BG Cross Section"]

### Need to cut off the first two leading bins for all of the jet pT-centric variables
### These leading two bins are passed through the unfolding to allow for migrations up into the leading bins
### Strategy here is just to use this number to alter what we see on the plot i.e. the axis ranges
# chop off 2 first bins for all non-w boson pt based distributions (due to cuts on jets and leptons in analysis code)
chopLeadingBins = 2

### Set as true if you want shaded bands as the total systematic uncertainty (added in quadrature)
doSystBands = True
#doSystBands = False

### Plots MG5 NLO FxFx and LO MLM gen cross sections
#doMadgraphGen = True
doMadgraphGen = False

###########################################################

ROOT.gStyle.SetHatchesSpacing(0.45);
ROOT.gStyle.SetHatchesLineWidth(1);

numVar = len(variables)
numNames = len(names)

print ("\nBegin!")
for iVar, variable in enumerate(variables):

    print("\n>>>>>>>>>> Doing variable: "+str(variable))
    histos = []
    histos_CentralRatio = []
    histos_PercentUncertainty = []

    ### Grab histo from each systematic variation ---
    for iName, name in enumerate(names):
        fileName = unfoldDir+"SMu_"+variable+"_"+name+".root"
        print ("Opening file: "+fileName)
        file = ROOT.TFile.Open(fileName, "READ")

        print ("-----> Getting UnfData hist with name #"+str(iName)+": "+name)
        hTemp = file.Get("hUnfData")
        #Using SetDirectory(0) is necessary, so when you close the file there's no error
        #Or use TH1::AddDirectory(kFALSE)?
        hTemp.SetDirectory(0)
        for iBin in range(1, hTemp.GetNbinsX()+1):
            bincountTemp = hTemp.GetBinContent(iBin)
            if (bincountTemp < 0.):
                print("---> Bin #"+str(iBin)+" count is below zero: "+str(bincountTemp))
                print("---> Setting bin content to 0.0000000000001")
                hTemp.SetBinContent(iBin, 0.0000000000001)
        histos.append(hTemp)

        if (file.IsOpen()):
            print ("File "+fileName+" is still open, closing...")
            file.Close()

    ## Get the MG5 NLO FxFx and LO MLM files ---
    if (doMadgraphGen):
        print ("-----> Getting MG5 signal samples")
        fileNameGen = "UnfoldedFiles_"+year+"/"+"SMu_unfolded_"+variable+"_TUnfold_JetPtMin_30_JetEtaMax_24_MGPYTHIA6_.root"
        print ("Opening file: "+fileNameGen)
        fileGen = ROOT.TFile.Open(fileNameGen, "READ")
        hGenNLOFXFX = fileGen.Get("hMadGenDYJetsCrossSection")
        hGenLOMLM = fileGen.Get("hGen1DYJetsCrossSection")
        hGenNLOFXFX.SetDirectory(0)
        hGenLOMLM.SetDirectory(0)
        if (fileGen.IsOpen()):
            print ("File "+fileNameGen+" is still open, closing...")
            fileGen.Close()

    ## Get x- and y-axis bounds ---
    hCentral = histos[0]
    numBins = hCentral.GetNbinsX()
    if (chopLeadingBins > 0):
        print("\nChopping off the first "+str(chopLeadingBins)+" bins from the distribution!")
        xmin = hCentral.GetXaxis().GetBinLowEdge(1+chopLeadingBins)
    else:
        xmin = hCentral.GetXaxis().GetXmin()
    xmax = hCentral.GetXaxis().GetXmax()

#    histMaxes = []
#    histMins = []
#    for iHist, hist in enumerate(histos):
#        histMaxes.append(hist.GetMaximum())
#        histMins.append(hist.GetMinimum())
#    ymax = max(histMaxes)
#    ymin = min(histMins)

    if (chopLeadingBins > 0):
        if (doRatio):
            ymin = hCentral.GetBinContent(1+chopLeadingBins)
            ymax = hCentral.GetBinContent(numBins)
        else:
            ### This is probably only going to work for steeply falling dist., like jet pT
            ymin = hCentral.GetBinContent(numBins)
            ymax = hCentral.GetBinContent(1+chopLeadingBins)
    else:
        ymin = hCentral.GetMinimum()
        ymax = hCentral.GetMaximum()

    print("\nNum. of bins: "+str(numBins)+", x-Min: "+str(xmin)+", x-Max: "+str(xmax)+", y-Min: "+str(ymin)+", y-Max: "+str(ymax))

    #########################

    c1 = ROOT.TCanvas("c1", "c1", 600, 800)
    c1.cd()
    pad1 = ROOT.TPad("pad1.1", "pad1.1", 0, 0.3, 1, 1)
    pad2 = ROOT.TPad("pad1.2", "pad1.2", 0, 0, 1, 0.3);
    pad1.SetTopMargin(0.075)
    pad1.SetBottomMargin(0)
    pad1.SetTicks()
    pad1.Draw()
    pad2.SetTopMargin(0)
    pad2.SetBottomMargin(0.3)
    pad2.SetTicks()
    pad2.Draw()

    ### Pad 1
    pad1.cd()
    if not (doRatio):
        pad1.SetLogy()

    htemp = ROOT.TH1D("htemp", "htemp", 100, xmin, xmax)
    htemp.SetStats(0)
    htemp.GetXaxis().SetTitle("")
    if (doRatio):
        htemp.GetYaxis().SetRangeUser(0.1, ymax+(abs(ymax)*0.3))
        htemp.GetYaxis().SetTitle("Ratio")
        htemp.SetTitle("Ratio of "+variable+", Unfolded")
    else:
        htemp.GetYaxis().SetRangeUser(ymin-(abs(ymin)*0.3), ymax+(abs(ymax)*2.5))
#        htemp.GetYaxis().SetRangeUser(ymin-(abs(ymin)*1.), ymax*2.)
#        htemp.GetYaxis().SetTitle("d#sigma/dp_{T}")
        htemp.GetYaxis().SetTitle("Diff. Cross Section")
        htemp.SetTitle(variable+", Unfolded")
    htemp.Draw()


    ### line and marker settings ---
    hCentral.SetLineColor(ROOT.kBlack)
    hCentral.SetLineWidth(1)
    hCentral.SetMarkerColor(ROOT.kBlack)
    hCentral.SetMarkerStyle(20)
    hCentral.SetMarkerSize(0.8)

    if (numNames > 1):
        j = 0
        colors = [ROOT.kOrange-3, ROOT.kGreen+1, ROOT.kBlue, ROOT.kRed+1, ROOT.kCyan, ROOT.kMagenta, ROOT.kAzure+8]
        for i in range(1, numNames):
            histos[i].SetLineWidth(1)
            histos[i].SetMarkerStyle(20)
            histos[i].SetMarkerSize(0.8)
            if (i % 2 == 1):
                j = j+1
            histos[i].SetLineColor(colors[j-1])
#            histos[i].SetLineColorAlpha(colors[j-1], 0.5)
            histos[i].SetMarkerColor(colors[j-1])

    if (doMadgraphGen):
        hGenNLOFXFX.SetLineColor(ROOT.kOrange)
        hGenNLOFXFX.SetLineWidth(1)
        hGenNLOFXFX.SetMarkerColor(ROOT.kOrange)
        hGenNLOFXFX.SetMarkerStyle(20)
        hGenNLOFXFX.SetMarkerSize(0.8)
        hGenLOMLM.SetLineColor(ROOT.kOrange+3)
        hGenLOMLM.SetLineWidth(1)
        hGenLOMLM.SetMarkerColor(ROOT.kOrange+3)
        hGenLOMLM.SetMarkerStyle(20)
        hGenLOMLM.SetMarkerSize(0.8)

######################################################################################################################
### Error calculations to make the shaded error bands
    if (numNames > 1):
        if (doSystBands):
            systErrsUp = []
            systErrsDown = []
            for i in range(1, numNames, 2):
                print("\n>>>>>>>>>> Doing errors for "+str(names[i])+" and "+str(names[i+1]))
                errsTempUp = []
                errsTempDown = []
                for jBin in range(1, numBins+1):
                    varCentral = histos[0].GetBinContent(jBin)
                    ### By grabbing these varMax and varMin, I'm disregarding which variation produced the up and the down
                    ### I believe that this means that I'm disregarding (anti)correlation, but we can compute covariance matrices later
                    varMax = max(varCentral, histos[i].GetBinContent(jBin), histos[i+1].GetBinContent(jBin))
                    varMin = min(varCentral, histos[i].GetBinContent(jBin), histos[i+1].GetBinContent(jBin))
                    print("Bin #"+str(jBin)+":")
                    print(names[0]+", "+str(varCentral))
                    print(names[i]+", "+str(histos[i].GetBinContent(jBin)))
                    print(names[i+1]+", "+str(histos[i+1].GetBinContent(jBin)))
                    print("Max: "+str(varMax)+", "+str(100.*(varMax-varCentral)/varCentral)+"% Up!")
                    print("Min: "+str(varMin)+", "+str(100.*(varMin-varCentral)/varCentral)+"% Down!"+"\n")
                    ### Take absolute value of deviations up/down because errors will eventually create TGraphAsymmErrors
                    errsTempUp.append(abs(varMax-varCentral))
                    errsTempDown.append(abs(varMin-varCentral))
                systErrsUp.append(errsTempUp)
                systErrsDown.append(errsTempDown)

#            print("systErrsUp: "+str(systErrsUp)+"\n")
#            print("systErrsDown: "+str(systErrsDown)+"\n")
    #        print(str(systErrsUp[1][0])+"\n") #first number is the systematic, second number is the bin

            ### Okay now that we've computed all bins' errors for each pair of variations, let's add them in quadrature
            systErrTotalUp = array('d')
            systErrTotalDown = array('d')
            ## Loop over the number of bins
            for iBin in range(0, numBins):
                sumUp = 0.
                sumDown = 0.
                ## Loop over the number of systematics
                for j in range(0, int((numNames-1)/2), 1):
                    sumUp += (systErrsUp[j][iBin])*(systErrsUp[j][iBin])
                    sumDown += (systErrsDown[j][iBin])*(systErrsDown[j][iBin])
                systErrTotalUp.append(math.sqrt(sumUp))
                systErrTotalDown.append(math.sqrt(sumDown))

            print("-----> Systematic Errors Added in Quadrature by Bin: ")
            print("-----> systErrTotalUp: "+str(systErrTotalUp))
            print("-----> systErrTotalDown: "+str(systErrTotalDown)+"\n")

            ### Now make graph that overlaps with the central distribution but with the syst error bands
            xsecCentral = array('d')
            binCenters = array('d')
            xErrLow = array('d')
            xErrUp = array('d')
            for iBin in range(1, numBins+1):
                xsecCentral.append(hCentral.GetBinContent(iBin))
                binCenters.append((hCentral.GetXaxis().GetBinLowEdge(iBin)+hCentral.GetXaxis().GetBinUpEdge(iBin))/2.)
                xErrLow.append(binCenters[iBin-1]-hCentral.GetXaxis().GetBinLowEdge(iBin))
                xErrUp.append(hCentral.GetXaxis().GetBinUpEdge(iBin)-binCenters[iBin-1])

            grCentralSystErrors = ROOT.TGraphAsymmErrors(numBins, binCenters, xsecCentral, xErrLow, xErrUp, systErrTotalDown, systErrTotalUp)
            grCentralSystErrors.SetLineWidth(1)
            grCentralSystErrors.SetLineStyle(1)
            grCentralSystErrors.SetFillStyle(3354)
            grCentralSystErrors.SetLineColor(ROOT.kBlack)
            grCentralSystErrors.SetFillColorAlpha(ROOT.kBlack, 0.6)
            grCentralSystErrors.SetMarkerSize(0.)
            grCentralSystErrors.Draw("E2 same") #draws just the hatched bands
            hCentral.Draw("PE same") #draws the points with statistical error bars

        else:
            for i in range(1, numNames):
                histos[i].Draw("P same")
            hCentral.Draw("PE same")

    if (doMadgraphGen):
        hGenNLOFXFX.Draw("PE SAME")
        hGenLOMLM.Draw("PE SAME")
######################################################################################################################

    ### draw legends
#    leg1 = ROOT.TLegend(0.3,0.7,0.935,0.975)
#    leg1 = ROOT.TLegend(0.7,0.5,0.935,0.975)
#    leg1 = ROOT.TLegend(0.675,0.40,0.9,0.925)
    if (doRatio):
        leg1 = ROOT.TLegend(0.625,0.,0.9,0.5)
    else:
        if (doMadgraphGen):
#            leg1 = ROOT.TLegend(0.525,0.30,0.9,0.925)
            leg1 = ROOT.TLegend(0.475,0.60,0.9,0.925) #10 may - just having gen dist in ratio subplot for now
        else:
            leg1 = ROOT.TLegend(0.625,0.30,0.9,0.925)

    leg1.AddEntry(hCentral, "Unfolded Central", "lpe")
    if (numNames > 1):
#        if (doSystBands):
#            leg1.AddEntry(grCentralSystErrors , "Total Syst. Uncert.", "f")
        if (doMadgraphGen):
            leg1.AddEntry(hGenNLOFXFX, "MG5_aMC FxFx + PY8 (#leq 2j NLO + PS)", "lpe")
            leg1.AddEntry(hGenLOMLM, "MG5_aMC MLM + PY8 (#leq 4j LO + PS)", "lpe")

        #10 may - just having gen dist in ratio subplot for now
        for i in range(1, numNames):
            leg1.AddEntry(histos[i], names[i], "lp")

    leg1.Draw("same")

    ### Pad 2
    pad2.cd()

    htemp1 = ROOT.TH1D("htemp1", "htemp1", 100, xmin, xmax)
    htemp1.SetStats(0)
    htemp1.GetXaxis().SetTitle("p_{T} [GeV]")
#    htemp1.GetXaxis().SetTitle("Angle")
    htemp1.GetXaxis().SetTitleOffset(1.1)
    htemp1.GetXaxis().SetTitleSize(0.09)
    htemp1.GetXaxis().SetLabelSize(0.09)
    htemp1.GetYaxis().SetTitle("Ratio to Unf. Central ")
    htemp1.GetYaxis().SetTitleOffset(0.475)
    htemp1.GetYaxis().SetTitleSize(0.09)
    htemp1.GetYaxis().SetLabelSize(0.06)
#    htemp1.GetYaxis().SetRangeUser(0.69, 1.32)
#    htemp1.GetYaxis().SetRangeUser(0.84, 1.16)
#    htemp1.GetYaxis().SetRangeUser(0.49, 1.52)
#    htemp1.GetYaxis().SetRangeUser(0.79, 1.22)
    htemp1.GetYaxis().SetRangeUser(0.4, 1.65)
    htemp1.SetTitle("")
    htemp1.Draw()



    ### Draw ratio subplot
    if (numNames > 1):
        for i in range(1, numNames):
            ratioTemp = histos[i].Clone()
            ratioTemp.Divide(hCentral)
            for jBin in range(1, numBins+1):
                ratioTemp.SetBinError(jBin, 0.)
            histos_CentralRatio.append(ratioTemp)
            histos_CentralRatio[i-1].Draw("P same") #10 may - just having gen dist in ratio subplot for now


    if (doMadgraphGen):
        ratioGenNLOFXFXData = hGenNLOFXFX.Clone()
        ratioGenLOMLMData = hGenLOMLM.Clone()
        ratioGenNLOFXFXData.Divide(hCentral)
        ratioGenLOMLMData.Divide(hCentral)
        ratioGenNLOFXFXData.Draw("PE SAME")
        ratioGenLOMLMData.Draw("PE SAME")

    line1 = ROOT.TLine(xmin, 1., xmax, 1.);
    line1.SetLineColor(ROOT.kBlack);
    line1.SetLineWidth(1);
    line1.Draw()

    #########################
    c1.Update()
    c1.Print(cwd+"/"+outputDir+"systPlot_"+variable+".pdf")
    if (c1):
        c1.Close()
        ROOT.gSystem.ProcessEvents()

    ## Delete TCanvas and such
    del c1, htemp, htemp1, leg1
######################################################################################################################
## Make percent uncertainty systematics plot
    if (doSystBands):
        c1 = ROOT.TCanvas("c1", "c1", 400, 400)
        c1.cd()
        pad1 = ROOT.TPad("pad1.1", "pad1.1", 0, 0., 1, 1)
        pad1.SetTicks()
        pad1.Draw()

        ### Pad 1
        pad1.cd()
        pad1.SetLogy()

        htemp = ROOT.TH1D("htemp", "htemp", 100, xmin+0.01, xmax)
#        htemp.GetYaxis().SetRangeUser(0., 30.)
        htemp.GetYaxis().SetRangeUser(0.01, 100.)
        htemp.GetYaxis().SetTitle("Percent Uncertainty")
        htemp.SetStats(0)
        htemp.GetXaxis().SetTitle("p_{T} [GeV]")
#        htemp.GetXaxis().SetTitle("Angle")
        htemp.SetTitle(variable+", Syst. Uncertainties")
        htemp.Draw()

        for i in range(0, int((numNames-1)/2), 1):
            hTemp_PercUncert = hCentral.Clone()
            for jBin in range(0, numBins):
                percentUncertainty = 0.
        #            print(max(systErrsUp[i][jBin], systErrsDown[i][jBin]))
        #            print(hCentral.GetBinContent(jBin+1))
        #            print( (max(systErrsUp[i][jBin], systErrsDown[i][jBin]))*100./hCentral.GetBinContent(jBin+1) )
        #            print("---")
                percentUncertainty = (max(systErrsUp[i][jBin], systErrsDown[i][jBin]))*100./(hCentral.GetBinContent(jBin+1))
                hTemp_PercUncert.SetBinContent(jBin+1, percentUncertainty)
                hTemp_PercUncert.SetBinError(jBin+1, 0.)
            hTemp_PercUncert.SetLineWidth(2)
            hTemp_PercUncert.SetMarkerStyle(20)
            hTemp_PercUncert.SetMarkerSize(0.6)
            hTemp_PercUncert.SetLineColor(colors[i])
            hTemp_PercUncert.SetMarkerColor(colors[i])
            histos_PercentUncertainty.append(hTemp_PercUncert)
            histos_PercentUncertainty[i].Draw("SAME")


        hTemp_PercUncert = hCentral.Clone()
        print("\n ----- Total Percent Uncertainty -----")
        for iBin in range(0, numBins):
            percentUncertainty = 0.
            percentUncertainty = (max(systErrTotalUp[iBin], systErrTotalDown[iBin]))*100./(hCentral.GetBinContent(iBin+1))
            hTemp_PercUncert.SetBinContent(iBin+1, percentUncertainty)
            hTemp_PercUncert.SetBinError(iBin+1, 0.)
            print("Bin #"+str(iBin+1)+": "+str(percentUncertainty)+" %")
        hTemp_PercUncert.SetLineWidth(2)
        hTemp_PercUncert.SetMarkerStyle(20)
        hTemp_PercUncert.SetMarkerSize(0.6)
        hTemp_PercUncert.SetLineColor(ROOT.kBlack)
        hTemp_PercUncert.SetMarkerColor(ROOT.kBlack)
        histos_PercentUncertainty.append(hTemp_PercUncert)
        histos_PercentUncertainty[int((numNames-1)/2)].Draw("SAME")

#        leg1 = ROOT.TLegend(0.625,0.50,0.9,0.9)
        leg1 = ROOT.TLegend(0.625,0.1,0.9,0.4)
        if (numNames > 1):
            leg1.AddEntry(histos_PercentUncertainty[int((numNames-1)/2)], "Total Syst. Uncertainty", "l")
            for i in range(0, int((numNames-1)/2), 1):
                leg1.AddEntry(histos_PercentUncertainty[i], namesLegend[i], "l")
        leg1.Draw("same")

        #########################
        c1.Update()
        c1.Print(cwd+"/"+outputDir+"systPlot_"+variable+"_Uncertainties"+".pdf")
        if (c1):
            c1.Close()
            ROOT.gSystem.ProcessEvents()

        ## Delete TCanvas and such
        del c1, htemp, leg1
    ## Delete large objects
    del histos, histos_CentralRatio
print ("\nFinished!")

