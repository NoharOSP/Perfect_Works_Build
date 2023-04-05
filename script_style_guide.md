# Xenogears Perfect Works Build
## Game Script Style Guide

### General
For all spelling, the target is US English.

### Numbers
- Amounts of things, measurements of time: 1-100 spelled out, 101+ as numerals (eg. three gates, twelve years, 500 years)
- Abbreviated, unabbreviated, or implicit measurements: always numerals (5 kph, 10 repsol per hour, 1000 sharls, incoming speed 120)
- Ages: always numerals (eg. aged 3, 7 years old, my son is 16)
- Money: always numerals (eg. 3000G)
- Game mechanics/counters: always numerals (eg. 2 AP, 3 Action Points, 1 HP)
- Percentages: always numerals (eg. 1%, 100%)
- Countdowns: always numerals (eg. 3...2...1..., commencing in 3...)
- Computer readouts: always numerals (eg you have 19 unread mails)
- Military divisions/buildings: always numerals/ordinals (3rd Fleet, 11th Development Base)
- Military manoeuvres/stages: always numerals (BattConn level 1, mark 3, pattern 10)
- Component/room/vehicle ID: always numerals (eg. torpedo tube 6, turret 2, bridge 3)

### Punctuation
- "Double quotes around complete dialogue where the speaker is named". No quotes around dialogue where the speaker is not named.
- Single quotes for 'quoted' or 'emphasised' terms in the dialogue of ANY character.
- Single quotes for direct reference to selecting a UI element (eg. select 'Arcane' to open a spells list).
- Key terms may be put in 'single quotes' when first introduced or explained. Once will usually be enough.
- Space after ellipses when at the end of a sentence... But no space when...the sentence continues like this.
- S-Stuttered words at the start of a sentence are capitalised before and after the dash, but otherwise n-not.
    - Before the dash, everything up to the first vowel of the stuttered word should be r-repeated, th-thanks.
- Asterisks around onomatopoeic words like \*huff\*, \*groan\*, but not around phonetics like argh, tch.
- Em dash with spaces for emphasis of explanatory clauses — this being one example.
- Parenthetical em dashes where — such as in this example — deemed suitable. Again include the spaces.
- Immediate em dash then a space for end of a line where someone is— Interrupted by themself or someone else.
- \- Dashes with spaces around headings for some computer readouts etc -
- Acronyms never have full stops, so DNA not D.N.A.
- Always okay, never OK or O.K.

### Titles
Use title case for the speakers of dialogue boxes, location names, items, in-game books etc:
- Mysterious Woman
- Man in Black
- Mountain Path
- Entrance to Shevat Palace
- Ether Doubler
- The Adventures of Big Joe

Capitalise where a title could be swapped out for the person's name without making the phrase grammatically incorrect, otherwise not. Examples:
- Yes, Commander.
- Go ask the commander.
- You're the doc, Doc.
- Will you be joining us, good Doctor?
- Young Master, how could you?
- Where is the young master?

Exceptions are:
- For royalty. Capitalise if it's a reference to a specific person of royalty, otherwise not:
    - This is Queen Zephyr?
    - Protect the Queen.
    - They say the Prince was lost.
	- I'm not sure if I can become king.
- For capital cities. Capitalise if it's a reference to a specific city, otherwise not:
    - Let's return to the Capital.
    - This is the Imperial Capital of Kislev.
    - We're so proud of our nation's capital! (Grey area?)
    - Does every nation have a capital?

### Control schemes/save operations
In terms of casing, follow the PlayStation TRC wherever possible:
- Left/right/up/down directional button(s)
- SELECT button
- START button
- X button etc (each face button requires use of a pointer, mapped to a special font character)
- L1/L2/L3/R1/R2/R3 button(s)
- Memory Card slot 1/2

(See section 7.0 in https://psx.arthus.net/sdk/Psy-Q/DOCS/TECHNOTE/mtrc13.pdf)

### Notes
When inserting em dashes into the script txt files, they must be typed as a double dash inside angled brackets like so <-->
This is so that Xenolater will map them correctly to the font file.