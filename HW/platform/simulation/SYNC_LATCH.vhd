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

entity SYNC_LATCH is
   generic
   (
      DATA_WIDTH : natural
   );
   port 
   (
      enable_in : IN STD_LOGIC;
      data_in   : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0);
      data_out  : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 DOWNTO 0)
   );
end SYNC_LATCH;

architecture rtl of SYNC_LATCH is
signal data_r:STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
begin

data_out <= data_r;

process(enable_in,data_in)
begin
   if(enable_in='1') then
      data_r <= data_in;
   end if;
end process;

end rtl;