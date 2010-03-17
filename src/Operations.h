// Operations.h
// Copyright (c) 2009, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.

#include "interface/ObjList.h"
#include "HeeksCNCTypes.h"

#pragma once

class COperations: public ObjList {
	static wxIcon* m_icon;
public:
	COperations() { }
	COperations & operator= ( const COperations & rhs );
	COperations( const COperations & rhs );

	// HeeksObj's virtual functions
	void CopyFrom(const HeeksObj *object);
	bool OneOfAKind(){return true;}
	int GetType()const{return OperationsType;}
	const wxChar* GetTypeString(void)const{return _("Operations");}
	HeeksObj *MakeACopy(void)const{ return new COperations(*this);}
	void GetIcon(int& texture_number, int& x, int& y){GET_ICON(13, 0);}
	bool CanAddTo(HeeksObj* owner){return owner->GetType() == ProgramType;}
	bool CanAdd(HeeksObj* object);
	bool CanBeRemoved(){return false;}
	void WriteXML(TiXmlNode *root);
	bool AutoExpand(){return true;}
	void GetTools(std::list<Tool*>* t_list, const wxPoint* p);
	bool UsesID() const { return(false); }
	void glCommands(bool select, bool marked, bool no_color);
	void ReloadPointers();

	static HeeksObj* ReadFromXMLElement(TiXmlElement* pElem);
};
