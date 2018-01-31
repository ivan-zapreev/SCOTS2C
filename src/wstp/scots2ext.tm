/*To launch this program from within Mathematica use:
 *   In[1]:=link=Install["./scots2ext"]
 *or launch this program from a shell and establish a
 *peer-to-peer connection. When given the prompt Create Link:
 *type a port name.(On Unix platforms, a port name is a*number
 *less than 65536. On Mac or Windows platforms, it's an
 *arbitrary word.) Then, from within Mathematica use:
 *   In[1]:=link=Install["portname",LinkMode->Connect]
 */

int load_controller_bdd P((const char *s));

:Begin:
:Function:		load_controller_bdd
:Pattern:		LoadSCOTSv2BDD[s_String]
:Arguments:		{s}
:ArgumentTypes:	{String}
:ReturnType:	Integer32
:End:

:Evaluate: LoadSCOTSv2BDD::usage "LoadSCOTSv2BDD[s] Allows to load a SCOTSv2.0 BDD controller, returns the error code"

int get_dim P(());

:Begin:
:Function:		get_dim
:Pattern:		GetDim[]
:Arguments:		{}
:ArgumentTypes:	{}
:ReturnType:	Integer32
:End:

:Evaluate: GetDim::usage "GetDim[] Get the total number of dimensions of the SCOTSv2.0 controller"

void get_eta P(());

:Begin:
:Function:		get_eta
:Pattern:		GetEta[]
:Arguments:		{}
:ArgumentTypes:	{}
:ReturnType:	Manual
:End:

:Evaluate: GetEta::usage "GetEta[] Get the vector of discretization parameters"

void get_lower_left P(());

:Begin:
:Function:		get_lower_left
:Pattern:		GetLowerLeft[]
:Arguments:		{}
:ArgumentTypes:	{}
:ReturnType:	Manual
:End:

:Evaluate: GetLowerLeft::usage "GetLowerLeft[] Get the lower-left grid coordinate"

void get_upper_right P(());

:Begin:
:Function:		get_upper_right
:Pattern:		GetUpperRight[]
:Arguments:		{}
:ArgumentTypes:	{}
:ReturnType:	Manual
:End:

:Evaluate: GetUpperRight::usage "GetUpperRight[] Get the upper-right grid coordinate"

void restriction P((double * point, const long len));

:Begin:
:Function:		restriction
:Pattern:		Restriction[x_List]
:Arguments:		{x}
:ArgumentTypes:	{RealList}
:ReturnType:	Manual
:End:

:Evaluate: Restriction::usage "Restriction[x] Get the input/control signals for the given point/state/position"
