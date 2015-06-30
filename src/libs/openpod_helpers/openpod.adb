-------------------------------------------------------------------
--
-- ADA support debug - print driver and device structure from ADA
--
-------------------------------------------------------------------


with Gnat.Io; use Gnat.Io;

package body OpenPOD is




procedure ada_print_drv( drv : access Pod_Driver ) is

begin

	Put( "Ada Driver structure print, drv name = '" );

	
	Put( Interfaces.C.Strings.Value( drv.name ) );

	Put( "'" );
	New_Line;


        return;

end ada_print_drv;

end OpenPOD;

