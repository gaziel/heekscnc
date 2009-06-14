// Drilling.h
/*
 * Copyright (c) 2009, Dan Heeks, Perttu Ahola
 * This program is released under the BSD license. See the file COPYING for
 * details.
 */

#include "Op.h"
#include "HeeksCNCTypes.h"

class CDrilling;

class CDrillingParams{
	
public:
	double m_standoff;		// This is the height above the staring Z position that forms the Z retract height (R word)
	double m_dwell;			// If dwell_bottom is non-zero then we're using the G82 drill cycle rather than G83 peck drill cycle.  This is the 'P' word
	double m_depth;			// Incremental length down from 'z' value at which the bottom of the hole can be found
	double m_peck_depth;		// This is the 'Q' word in the G83 cycle.  How deep to peck each time.

	int m_cutting_tool_number;	// Reference into CuttingTool object from whence radius comes

	// The following line is the prototype setup in the Python routines for the drill sequence.
	// def drill(x=None, y=None, z=None, depth=None, standoff=None, dwell=None, peck_depth=None):

	void set_initial_values( const std::list< std::pair<int, int> >  & cuttingTools );
	void write_values_to_config();
	void GetProperties(CDrilling* parent, std::list<Property *> *list);
	void WriteXMLAttributes(TiXmlNode* pElem);
	void ReadParametersFromXMLElement(TiXmlElement* pElem);
};

/**
	The CDrilling class stores a list of symbols (by type/id pairs) of elements that represent the starting point
	of a drilling cycle.  In the first instance, we use PointType objects as starting points.  Rather than copy
	the PointType elements into this class, we just refer to them by ID.  In the case of PointType objects,
	the class assumes that the drilling will occur in the negative Z direction.

	One day, when I get clever, I intend supporting the reference of line elements whose length defines the
	drill's depth and whose orientation describes the drill's orientation at machining time (i.e. rotate A, B and/or C axes)
 */

class CDrilling: public COp {
private:
	/**
		There are all types of 3d point classes around but most of them seem to be in the HeeksCAD code
		rather than in cod that's accessible by the plugin.  I suspect I'm missing something on this
		but, just in case I'm not, here is a special one (just for this class)
	 */
	typedef struct Point3d {
		double x;
		double y;
		double z;
		Point3d( double a, double b, double c ) : x(a), y(b), z(c) { }
	} Point3d;

	/**
		The following two methods are just to draw pretty lines on the screen to represent drilling
		cycle activity when the operator selects the Drilling Cycle operation in the data list.
	 */
	std::list< Point3d > PointsAround( const Point3d & origin, const double radius, const unsigned int numPoints ) const;
	std::list< Point3d > DrillBitVertices( const Point3d & origin, const double radius, const double length ) const;

public:
	/**
		Define some data structures to hold references to CAD elements.  We store both the type and id because
			a) the ID values are only relevant within the context of a type.
			b) we don't want to limit this class to PointType elements alone.  I can imagine
			   using it to identify pairs of intersecting elements and placing a drilling cycle
			   at their intersection (again, one day when I grow up)
 	 */
	typedef int SymbolType_t;
	typedef int SymbolId_t;
	typedef std::pair< SymbolType_t, SymbolId_t > Symbol_t;
	typedef std::list< Symbol_t > Symbols_t;

public:
	//	These are references to the CAD elements whose position indicate where the Drilling Cycle begins.
	Symbols_t m_symbols;
	CDrillingParams m_params;

	//	Constructors.
	CDrilling():COp(GetTypeString()){}
	CDrilling(const Symbols_t &symbols, const Symbols_t &cuttingTools):COp(GetTypeString()), m_symbols(symbols) { m_params.set_initial_values(cuttingTools);  }

	// HeeksObj's virtual functions
	int GetType()const{return DrillingType;}
	const wxChar* GetTypeString(void)const{return _T("Drilling");}
	void glCommands(bool select, bool marked, bool no_color);

	// TODO Draw a drill cycle icon and refer to it here.
	wxString GetIcon(){if(m_active)return theApp.GetResFolder() + _T("/icons/adapt"); else return COp::GetIcon();}
	void GetProperties(std::list<Property *> *list);
	HeeksObj *MakeACopy(void)const;
	void CopyFrom(const HeeksObj* object);
	void WriteXML(TiXmlNode *root);
	bool CanAddTo(HeeksObj* owner);

	// This is the method that gets called when the operator hits the 'Python' button.  It generates a Python
	// program whose job is to generate RS-274 GCode.
	void AppendTextToProgram();

	static HeeksObj* ReadFromXMLElement(TiXmlElement* pElem);

	void AddSymbol( const SymbolType_t type, const SymbolId_t id ) { m_symbols.push_back( Symbol_t( type, id ) ); }

};



