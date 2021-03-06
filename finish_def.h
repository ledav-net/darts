/*
 * Copyright (C) 2002 David De Grave <david@ledav.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _FINISH_DEF_H
#define _FINISH_DEF_H

const	char	*bestfinish[171] =
{
	/*   0 */	"Won !",
	/*   1 */	"No way !",
	/*   2 */	"D1",
	/*   3 */	"1, D1",
	/*   4 */	"D2",
	/*   5 */	"1, D2",
	/*   6 */	"D3",
	/*   7 */	"3, D2",
	/*   8 */	"D4",
	/*   9 */	"1, D4",
	/*  10 */	"D5 or 2, D4",
	/*  11 */	"3, D4",
	/*  12 */	"D6 or 4, D4",
	/*  13 */	"1, D6 or 5, D4",
	/*  14 */	"D7",
	/*  15 */	"5, D5",
	/*  16 */	"D8",
	/*  17 */	"1, D8",
	/*  18 */	"D9 or 2, D8",
	/*  19 */	"3, D8",
	/*  20 */	"D10",
	/*  21 */	"1, D10 or 5, D4",
	/*  22 */	"D11 or 2, D10",
	/*  23 */	"7, D8",
	/*  24 */	"D12 or 8, D8",
	/*  25 */	"5, D10",
	/*  26 */	"D13 or 6, D10",
	/*  27 */	"7, D10",
	/*  28 */	"D14",
	/*  29 */	"1, D14",
	/*  30 */	"D15",
	/*  31 */	"1, D15",
	/*  32 */	"D16",
	/*  33 */	"1, D16",
	/*  34 */	"D17",
	/*  35 */	"3, D16",
	/*  36 */	"D18",
	/*  37 */	"5, D16",
	/*  38 */	"D19 or 6, D16",
	/*  39 */	"7, D16",
	/*  40 */	"TOP",
	/*  41 */	"9, D16",
	/*  42 */	"10, D16",
	/*  43 */	"3, TOP",
	/*  44 */	"12, D16",
	/*  45 */	"13, D16",
	/*  46 */	"6, TOP",
	/*  47 */	"15, D16",
	/*  48 */	"16, D16",
	/*  49 */	"17, D16",
	/*  50 */	"BULL or 10, TOP",
	/*  51 */	"19, D16",
	/*  52 */	"12, TOP",
	/*  53 */	"13, TOP",
	/*  54 */	"14, TOP",
	/*  55 */	"15, TOP",
	/*  56 */	"16, TOP",
	/*  57 */	"17, TOP",
	/*  58 */	"18, TOP",
	/*  59 */	"19, TOP",
	/*  60 */	"20, TOP",
	/*  61 */	"T15, D8",
	/*  62 */	"T10, D16",
	/*  63 */	"T13, D12",
	/*  64 */	"T16, D8",
	/*  65 */	"T19, D4",
	/*  66 */	"T10, D18",
	/*  67 */	"T17, D8",
	/*  68 */	"T20, D4",
	/*  69 */	"19, BULL",
	/*  70 */	"T18, D8",
	/*  71 */	"T13, D16",
	/*  72 */	"T16, D12",
	/*  73 */	"T19, D8",
	/*  74 */	"T14, D16",
	/*  75 */	"T13, D18",
	/*  76 */	"T10, D18",
	/*  77 */	"T15, D16",
	/*  78 */	"T18, D12",
	/*  79 */	"T13, TOP",
	/*  80 */	"T16, D16",
	/*  81 */	"T15, D18",
	/*  82 */	"T14, TOP",
	/*  83 */	"T17, D16",
	/*  84 */	"T16, D18",
	/*  85 */	"T15, TOP",
	/*  86 */	"T18, D16",
	/*  87 */	"T17, D18",
	/*  88 */	"T16, TOP",
	/*  89 */	"T19, D16",
	/*  90 */	"T18, D18",
	/*  91 */	"T17, TOP",
	/*  92 */	"T20, D16",
	/*  93 */	"T19, D18",
	/*  94 */	"T18, TOP",
	/*  95 */	"T19, D19",
	/*  96 */	"T20, D18",
	/*  97 */	"T19, TOP",
	/*  98 */	"T20, D19",
	/*  99 */	"D19, 10, D16",
	/* 100 */	"T20, TOP",
	/* 101 */	"T17, 10, TOP",
	/* 102 */	"T20, 10, D16",
	/* 103 */	"T19, 10, D18",
	/* 104 */	"T20, 12, D16",
	/* 105 */	"T20, 13, D16",
	/* 106 */	"T20, 10, D18",
	/* 107 */	"T19, 10, TOP",
	/* 108 */	"T20, 16, D16",
	/* 109 */	"T19, 12, TOP",
	/* 110 */	"T20, 10, TOP",
	/* 111 */	"T20, 19, D16",
	/* 112 */	"T20, 12, TOP",
	/* 113 */	"T20, 13, TOP",
	/* 114 */	"T20, 14, TOP",
	/* 115 */	"T20, 15, TOP",
	/* 116 */	"T20, 16, TOP",
	/* 117 */	"T20, 17, TOP",
	/* 118 */	"T20, 18, TOP",
	/* 119 */	"T20, 19, TOP",
	/* 120 */	"T20, 20, TOP",
	/* 121 */	"T19, 14, BULL",
	/* 122 */	"T18, 18, BULL",
	/* 123 */	"T20, T13, D12",
	/* 124 */	"T20, T16, D8",
	/* 125 */	"T20, T19, D4",
	/* 126 */	"T19, 19, BULL",
	/* 127 */	"T20, T17, D8",
	/* 128 */	"T20, T20, D4",
	/* 129 */	"T19, T16, D12",
	/* 130 */	"T20, T18, D8",
	/* 131 */	"T20, T13, D16",
	/* 132 */	"T20, T16, D12",
	/* 133 */	"T20, T19, D8",
	/* 134 */	"T20, T14, D16",
	/* 135 */	"T20, T13, D18",
	/* 136 */	"T20, T20, D8",
	/* 137 */	"T19, T18, D16",
	/* 138 */	"T20, T14, D16",
	/* 139 */	"T20, T17, D14",
	/* 140 */	"T20, T20, D10",
	/* 141 */	"T20, T17, D15",
	/* 142 */	"T20, T20, D11",
	/* 143 */	"T20, T17, D16",
	/* 144 */	"T20, T20, D12",
	/* 145 */	"T20, T15, TOP",
	/* 146 */	"T20, T18, D16",
	/* 147 */	"T20, T17, D18",
	/* 148 */	"T20, T20, D14",
	/* 149 */	"T20, T19, D16",
	/* 150 */	"T20, T18, D18",
	/* 151 */	"T20, T17, TOP",
	/* 152 */	"T20, T20, D16",
	/* 153 */	"T20, T19, D18",
	/* 154 */	"T18, T20, TOP",
	/* 155 */	"T20, T19, D19",
	/* 156 */	"T20, T20, D18",
	/* 157 */	"T19, T20, TOP",
	/* 158 */	"T20, T20, D19",
	/* 159 */	"No way !",
	/* 160 */	"T20, T20, TOP",
	/* 161 */	"T20, T17, BULL",
	/* 162 */	"No way !",
	/* 163 */	"No way !",
	/* 164 */	"T19, T19, BULL",
	/* 165 */	"No way !",
	/* 166 */	"No way !",
	/* 167 */	"T20, T19, BULL",
	/* 168 */	"No way !",
	/* 169 */	"No way !",
	/* 170 */	"T20, T20, BULL"
};

#endif
