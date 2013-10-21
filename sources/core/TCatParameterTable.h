/* $Id:$ */
/**
 * @file   TCatParameterTable.h
 * @date   Created : Oct 07, 2012 11:07:51 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPARAMETERTABLE_H
#define TCATPARAMETERTABLE_H

#include <TGraph.h>
#include <TSpline.h>


class TCatParameterTable  : public TGraph {

public:
   TCatParameterTable();
   virtual ~TCatParameterTable();



   TCatParameterTable(const char* filename, const char* format, Option_t *option = "", Bool_t xIsFirst = kTRUE);

   void ScaleY(const Double_t &scale);

   virtual Double_t EvalY(const Double_t &x) const { return fSplineXY->Eval(x); }
   virtual Double_t EvalX(const Double_t &y) const { return fSplineYX->Eval(y); }

   // get filename
   const char* GetFilename() const { return fFilename.Data(); }

   TCatParameterTable(const TCatParameterTable& rhs);
   virtual TCatParameterTable& operator=(const TCatParameterTable& rhs);
   virtual void Copy(TObject &obj) const;


protected:
   virtual void SetPoint(Int_t i, Double_t x, Double_t y) { TGraph::SetPoint(i,x,y); }
   virtual void BuildSpline();
   

private:
   TString  fFilename; // filename
   TString  fFormat;   // format string
   Double_t fScaleY;
   Bool_t   fXIsFirst;
   TSpline* fSplineXY;   // spline to evaluate Y from X
   TSpline* fSplineYX;   // spline to evaluate X from Y
   
   ClassDef(TCatParameterTable,1);
};
#endif // end of #ifdef TCATPARAMETERTABLE_H
