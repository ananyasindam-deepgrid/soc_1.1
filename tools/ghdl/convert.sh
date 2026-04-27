#!/usr/bin/env bash

set -e

cd $(dirname "$0")

DGRID_RTL=../../HW/src

GHDL=./ghdl/bin/ghdl

rm -r -f build

rm -f *.v

mkdir -p build

# Import sources
$GHDL -i --std=08 --work=work --workdir=build -Pbuild \
  "$DGRID_RTL"/*.vhd \
  "$DGRID_RTL"/alu/*.vhd \
  "$DGRID_RTL"/dp/*.vhd \
  "$DGRID_RTL"/ialu/*.vhd \
  "$DGRID_RTL"/pcore/*.vhd \
  "$DGRID_RTL"/soc/axi/*.vhd \
  "$DGRID_RTL"/soc/peripherals/time.vhd \
  "$DGRID_RTL"/soc/peripherals/gpio.vhd \
  "$DGRID_RTL"/soc/peripherals/uart.vhd \
  "$DGRID_RTL"/soc/peripherals/vga.vhd \
  "$DGRID_RTL"/soc/peripherals/camera.vhd \
  "$DGRID_RTL"/util/shifter_l.vhd \
  "$DGRID_RTL"/util/shifter.vhd \
  "$DGRID_RTL"/util/multiplier.vhd \
  "$DGRID_RTL"/util/delayv.vhd \
  "$DGRID_RTL"/util/delayi.vhd \
  "$DGRID_RTL"/util/delay.vhd \
  "$DGRID_RTL"/util/arbiter.vhd \
  "$DGRID_RTL"/util/adder.vhd \
  "$DGRID_RTL"/util/afifo.vhd \
  "$DGRID_RTL"/util/afifo2.vhd \
  "$DGRID_RTL"/util/ram2r1w.vhd \
  "$DGRID_RTL"/util/ramw2.vhd \
  "$DGRID_RTL"/util/ramw.vhd \
  "$DGRID_RTL"/util/fifo.vhd \
  "$DGRID_RTL"/util/fifow.vhd \
  "$DGRID_RTL"/soc/*.vhd \
  "$DGRID_RTL"/top/*.vhd

# Top entity
$GHDL -m --std=08 --work=work --workdir=build soc_base 

# Synthesize: generate Verilog output
$GHDL synth --std=08 --work=work --workdir=build -Pbuild --out=verilog soc_base > soc.v
