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

entity multiplier is
	generic 
	(
        DATA_WIDTH      : natural;
        REGISTER_OUTPUT : BOOLEAN
	);
	port 
	(
        clock_in   : in std_logic;
        reset_in   : in std_logic;
        x_in       : in std_logic_vector(DATA_WIDTH-1 downto 0);
        y_in       : in std_logic_vector(DATA_WIDTH-1 downto 0);       
        z_out      : out std_logic_vector(2*DATA_WIDTH-1 downto 0)
	);
end multiplier;

architecture rtl of multiplier is
signal x   : signed(DATA_WIDTH-1 downto 0);
signal y   : signed(DATA_WIDTH-1 downto 0);
signal z   : signed(2*DATA_WIDTH-1 downto 0);
signal z_r : signed(2*DATA_WIDTH-1 downto 0);
begin

x <= signed(x_in);
y <= signed(y_in);
z <= x*y;

GEN1: if REGISTER_OUTPUT=TRUE generate
z_out <= std_logic_vector(z_r);
end generate GEN1;

GEN2: if REGISTER_OUTPUT=FALSE generate
z_out <= std_logic_vector(z);
end generate GEN2;

process(clock_in,reset_in)
begin
   if(reset_in='0') then
   else
      if(rising_edge(clock_in)) then 
         z_r <= z;
      end if;
   end if;
end process;

end rtl;
