void plotGeometries() {

    TFile fComp("../Data/Geometry_Moved.root");
    TFile fBase("../Data/Geometry_Baseline.root");

    TList * listComp = (TList *)((TCanvas *)fComp.Get("img041"))->GetListOfPrimitives();
    TList * listBase = (TList *)((TCanvas *)fBase.Get("img041"))->GetListOfPrimitives();

    TList * listFinal = new TList();

    TObjLink * lnk = listBase->FirstLink();
    TObjLink * lnk2 = listComp->FirstLink();
    while (lnk && lnk2) {
        listFinal->AddLast(lnk->GetObject());
        TObject * obj = lnk2->GetObject();
        if (obj->InheritsFrom("TPolyLine")) {
            TPolyLine * obj2 = (TPolyLine*)obj;
            obj2->SetLineColorAlpha(obj2->GetLineColor(), 0.5);
            listFinal->AddLast(obj2);
        }
        lnk = lnk->Next();
        lnk2 = lnk2->Next();
    }

    TCanvas * cnew = new TCanvas("c", "c", 1600, 600);

    listFinal->Draw();

    TPaveText * legendcomplement = new TPaveText(0.26, 0.75, 0.5, 0.85, "NDC");
    legendcomplement->AddText("Transparent lines:");
    legendcomplement->AddText(" Third disk -15 mm in Z");
    legendcomplement->Draw("SAME");
}