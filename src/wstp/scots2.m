(* ::Package:: *)

BeginPackage[ "SCOTSv2`"]
	Needs["GUIKit`"]

	LoadBDDController
	LoadBDDController::usage = "LoadBDDController[] allows to load the SCOTSv2.0 controller."

	Begin[ "`Private`"]
		LoadBDDController[] := Block[ {},
			(*Find the SCOTS v2.0 controller file*)
			fileName = SystemDialogInput["FileOpen",
										{"", {"Controller Files (*.scs)" -> {"*.scs"}}},
										WindowTitle -> "Select the SCOTSv2.0 controller to load"];
			If[fileName =!= $Canceled,
				(*Remove the .scs suffix from the controller's name*)
				fileTempl = StringTrim[fileName, RegularExpression["\\.scs$"]];
				(*Load the controller using the WSTP application*)
				res = Global`LoadSCOTSv2BDD[fileTempl];
			];
		]
	End[]
EndPackage[]










