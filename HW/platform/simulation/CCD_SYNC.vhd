------------------------------------------------------------------------------
-- Copyright [2026] [Deepgrid Semi Pvt Ltd]
--
-- Author: Deepgrid Semi Pvt Ltd
------------------------------------------------------------------------------

---
-- Synchronize a signal accross different clock domain
---

library std;
use std.standard.all;
LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.all;

entity CCD_SYNC is
   generic 
   (
      WIDTH  : natural
   );
   port 
   (
      SIGNAL reset_in    : in std_logic;
      SIGNAL inclock_in  : in std_logic;
      SIGNAL outclock_in : in std_logic;
      SIGNAL input_in    : in std_logic_vector(WIDTH-1 downto 0);
      SIGNAL output_out  : out std_logic_vector(WIDTH-1 downto 0)
   );
end CCD_SYNC;

architecture rtl of CCD_SYNC is
signal input_r:std_logic_vector(WIDTH-1 downto 0);
signal input_rr:std_logic_vector(WIDTH-1 downto 0);
signal input_rrr:std_logic_vector(WIDTH-1 downto 0);
begin

output_out <= input_rrr;

process(inclock_in,reset_in)
begin
   if(reset_in='0') then
      input_r <= (others=>'0');
      input_rr <= (others=>'0');
      input_rrr <= (others=>'0');
   else
      if(rising_edge(inclock_in)) then 
         input_r <= input_in;
         input_rr <= input_r;
         input_rrr <= input_rr;     
      end if;
   end if;
end process;

end rtl;