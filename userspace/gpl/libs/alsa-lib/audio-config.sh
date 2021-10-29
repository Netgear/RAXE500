#!/bin/sh

case "$1" in
	start)
		echo "Power up MapleLeaf sound card..."
		amixer cset -q name='HPL Output Mixer L_DAC Switch' 1
		amixer cset -q name='HPR Output Mixer R_DAC Switch' 1
		amixer cset -q name='HP DAC Playback Switch' 1 1
		amixer cset -q name='REF Power-up Cfg' 1
		exit 0
		;;

	stop)
		echo "Power down MapleLeaf sound card..."
		amixer cset -q name='HPL Output Mixer L_DAC Switch' 0
		amixer cset -q name='HPR Output Mixer R_DAC Switch' 0
		amixer cset -q name='HP DAC Playback Switch' 0 0
		amixer cset -q name='REF Power-up Cfg' 0
		exit 1
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

