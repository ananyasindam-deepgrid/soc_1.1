------------------------------------------------------------------------------
-- Copyright [2026] [Deepgrid Semi Pvt Ltd]
--
-- Author: Deepgrid Semi Pvt Ltd
------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity adder is
   generic
   (
      DATA_WIDTH : natural
   );
   port 
   (
      x_in       : in std_logic_vector((DATA_WIDTH-1) downto 0);
      y_in       : in std_logic_vector((DATA_WIDTH-1) downto 0);
      add_sub_in : in std_logic;
      z_out      : out std_logic_vector((DATA_WIDTH-1) downto 0)
   );
end entity;

architecture rtl of adder is

signal x:signed(DATA_WIDTH-1 downto 0);
signal y:signed(DATA_WIDTH-1 downto 0);
signal z:signed(DATA_WIDTH-1 downto 0);

begin

x <= signed(x_in);
y <= signed(y_in);
z_out <= std_logic_vector(z);

process(x,y,add_sub_in)
begin
   -- Add if "add_sub" is 1, else subtract
   if (add_sub_in = '1') then
      z <= x + y;
   else
      z <= x - y;
   end if;
end process;

end rtl;