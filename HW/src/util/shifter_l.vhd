------------------------------------------------------------------------------
-- Copyright [2026] [Deepgrid Semi Pvt Ltd]
--
-- Author: Deepgrid Semi Pvt Ltd
------------------------------------------------------------------------------

library std;
use std.standard.all;
LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.all;
use work.dgrid_pkg.all;

entity barrel_shifter_l is
   generic
   (
      DIST_WIDTH : natural;
      DATA_WIDTH : natural
   );
   port 
   (
      direction_in : in std_logic;
      data_in      : in std_logic_vector((DATA_WIDTH-1) downto 0);
      distance_in  : in std_logic_vector((DIST_WIDTH-1) downto 0);
      data_out     : out std_logic_vector((DATA_WIDTH-1) downto 0)
   );
end entity;

architecture rtl of barrel_shifter_l is
signal distance:unsigned(DIST_WIDTH-1 downto 0);
signal shift_left:std_logic_vector((DATA_WIDTH-1) downto 0);
signal shift_right:std_logic_vector((DATA_WIDTH-1) downto 0);
begin

distance <= unsigned(distance_in);
data_out <= shift_right when (direction_in = '1') else shift_left; 

sra_i : SHIFT_RIGHT_L
   GENERIC MAP (
      DATA_WIDTH=>DATA_WIDTH,
      DIST_WIDTH=>DIST_WIDTH
   )
   PORT MAP (
      data_in=>data_in,
      distance_in=>distance,
      data_out=>shift_right
   );

sla_i : SHIFT_LEFT_L
   GENERIC MAP (
      DATA_WIDTH=>DATA_WIDTH,
      DIST_WIDTH=>DIST_WIDTH
   )
   PORT MAP (
      data_in=>data_in,
      distance_in=>distance,
      data_out=>shift_left
   );

end rtl;
