------------------------------------------------------------------------------
-- Copyright [2026] [Deepgrid Semi Pvt Ltd]
--
-- Author: Deepgrid Semi Pvt Ltd
------------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Implementing asynchronous FIFO
-- Implementation is based on design as described in
-- http://www.sunburst-design.com/papers/CummingsSNUG2002SJ_FIFO1.pdf
-----------------------------------------------------------------------------

library std;
use std.standard.all;
LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.all;
use work.dgrid_pkg.all;

entity afifo2 is
   generic 
   (
        DATA_WIDTH  : natural;
        FIFO_DEPTH  : natural
   );
   port 
   (
        rclock_in       : in std_logic;
        wclock_in       : in std_logic;
        reset_in        : in std_logic;
        data_in         : in std_logic_vector(DATA_WIDTH-1 downto 0);
        write_in        : in std_logic;
        read_in         : in std_logic;
        q_out           : out std_logic_vector(DATA_WIDTH-1 downto 0);
        empty_out       : out std_logic;
        full_out        : out std_logic
   );
end afifo2;

architecture rtl of afifo2 is

SIGNAL data:STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
SIGNAL q:STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
SIGNAL write:STD_LOGIC;
SIGNAL empty:STD_LOGIC; 
SIGNAL full:STD_LOGIC;
SIGNAL read:STD_LOGIC;

begin

GEN1:if (FIFO_DEPTH > 5) generate
afifo_i:afifo
   generic map
   (
      DATA_WIDTH=>DATA_WIDTH,
      FIFO_DEPTH=>5
   )
   port map 
   (
      rclock_in=>rclock_in,
      wclock_in=>wclock_in,
      reset_in=>reset_in,
      data_in=>data,
      write_in=>write,
      read_in=>read_in,
      q_out=>q_out,
      empty_out=>empty_out,
      full_out=>full
   );

data <= q;
write <= (not empty) and (not full);
read <= write;

fifo_i:scfifo
   generic map
   (
      DATA_WIDTH=>DATA_WIDTH,
      FIFO_DEPTH=>FIFO_DEPTH,
      LOOKAHEAD=>TRUE
   )
   port map 
   (
      reset_in=>reset_in,
      clock_in=>wclock_in,
      read_in=>read,
      q_out=>q,
      empty_out=>empty,
      data_in=>data_in,
      write_in=>write_in,
      full_out=>full_out,
      ravail_out=>open,
      wused_out=>open,
      almost_full_out=>open
   );
end generate GEN1;

GEN2:if (FIFO_DEPTH <= 5) generate
afifo_i:afifo
   generic map
   (
      DATA_WIDTH=>DATA_WIDTH,
      FIFO_DEPTH=>FIFO_DEPTH
   )
   port map 
   (
      rclock_in=>rclock_in,
      wclock_in=>wclock_in,
      reset_in=>reset_in,
      data_in=>data_in,
      write_in=>write_in,
      read_in=>read_in,
      q_out=>q_out,
      empty_out=>empty_out,
      full_out=>full_out
   );

end generate GEN2;

end rtl;
