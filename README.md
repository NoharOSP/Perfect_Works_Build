# Perfect_Works_Build
Xenogears Perfect Works Build: Including all official Xenogears information in PSX ROM and other improvements.

Download version 0.1.2 here: 

- Mega: https://mega.nz/file/OG5RBJ4Q#jARzVJaRI1mhaYFWj9fMc2-TfDL5yl6abiJIdXY5lSU
- Google Drive: https://drive.google.com/file/d/1bGlPBH7_1J-61nnAsWWU9viVBNsFmNKo/view?usp=sharing

## Completion stats:

- Name/terminology changes: 100%
- Main script: 30% (up to escape from Noatun)
- NPC script: Not done (except for name/terminology changes)

## Coming soon:

- Name swapper
- Patch undo

## Introduction:

Hello. My name is Ryan (↙ ➡ ↘ ⬇ ↙ ⬅ ↘) Piccuito. I am a character writer and handler for game balance on the Xenogears Perfect Works mod. Xenogears: Perfect Works is a collaborative project aiming to make Xenogears a more complete experience. This does contain unmarked but mild spoilers, so readers are advised and warned not to scroll through this if they’re looking to use this mod to enjoy the game for the first time, or you simply never got to into it for various reasons we hope to address. These spoilers are primarily for certain story events in the story bits and lines are relegated to story specific changes and certain boss fights dungeons and all moves party members and availability are mentioned in the gameplay focused changes. 

DISCLAIMER: Some of the patches were designed to work in tandem with each other. The exp/gold patch was designed to work with the lower encounter rate patch. The monster stats, character stats and items patches all act as a rebalancing hack. They can be used separately from each other, but they were designed with the mindset of using them together.

## Version history:

### Version 0.1.2:

10/02/2022

- Issues regarding Beetle PSX HW have been resolved.
- CrossPendent has been renamed Cross.
- Sewer Key description has been corrected to say "Noatun" over "Nortune".
- Subtitles in the opening FMV have been corrected to last longer during the intercom voice announcement.
- Minor pre-Noatun typos, grammar issues and text wrapping fixes have been applied.
- Silver Blood and Black Snake shop descriptions have been swapped.
- "New Game" is used over "NewGame" in the start menu in the revised script.
- Babel Tower Bladegash's HP has been lowered from 22000 to 14000.
- Skyghene's HP has been lowered from 18000 to 16000. Its Ether is back to 50.
- Grandgrowl's HP has been lowered from 20000 to 18000. Its Attack is back to 500 and its Ether is back to 60.
- Tutorial for Hyper Mode has been added. Jessie will explain it before Babel Tower in the Gun Room.

### Version 0.1.1:

09/07/2022

- Fixes fatal glitch where exp/gold and monster patches won't work with the script patch.
- Fixes fatal glitch where encounters patch won't work with the items and script patches together.
- Fixes minor typos and text wrapping up to the Av Desert.

### Version 0.1.0:

09/04/2022

- Initial release

## Archive:

### GUI patcher:

#### Version 0.2.1

x86:
https://mega.nz/file/KHZQQCSD#vKSzJEudiouUPKzCzx9mxYOwGu4HFgIBq29fjFR3qo4

x64:
https://mega.nz/file/XSI32A4S#U1gV_X7pE8KjTZ0NY7Tb4oEZelk-OtKqzizgrUWk_PU

#### Version 0.1

DISCLAIMER: Only x64 is supported.

https://mega.nz/file/mWYViJTR#_a1CaUwPLeMviQ5ZwTs9kPjYC60c_yOjHd5s68oGbCs

## Instructions:

### Patching:

- Open the Xenogears: Perfect Works Patcher.
- Open the ISO files for both discs.
- Select which patches you wish to apply.
- Press Patch. Script patches will take over five minutes to apply due to the FMV data associated with them.

### Regenerating:

This step applies if you're playing the game using Beetle PSX HW and multiple patches have been applied.

- Download ECCRegen.
- Click the folder icon to select the Xenogears ISO.
- Click Scan.
- Wait for the unregenerated files to load.
- Right click Check all.
- Click Regenerate.
- Wait for the regeneration to finish.

## Saves:

For those who want to test a certain section in the game. They use the mcd format supported by Duckstation.

- xenomem1: Mountain Path - Operation Av
- xenomem2: The Tournament - Brave Battlers
- xenomem3: Brave Battlers - Secret Weapon
- xenomem4: Men of the Sea - Ocean Floor
- xenomem5: Deep Sea Girl - Protect Nisan!
- xenomem6: Protect Nisan! - Tears for Fears
- xenomem7: Shot Down! - Dreams...
- xenomem8: Fallen Star - First and Last (Merkabah)
- xenomemdebug: All debug rooms (disc 1 only)

## Patches:

### Encounters:

- Encounter rate has been halved except for the Duneman Isle, Shevat Emergency Shaft, Anima Dungeon 2 and Deus for grinding reasons.

Throughout the game, encounter rates have been lowered. For many players, this has been a strikingly annoying part of the game. Especially for sections with platforming as in emulated versions of Xenogears players no longer can hear the disc rev up loading the battle. Because the jump input gets eaten when a battle is being loaded, this will have you run off cliffs which is very frustrating. So the encounter rate was halved across a majority of the game. Select areas such as Deus, Anima Dungeon 2, the Duneman Isles, and the Shevat Emergency Shaft were kept the same. Deus is a common experience grinding spot due to the Power Seraph giving the most exp out of any single enemy in the game. Shevat’s Emergency Shaft and Anima Dungeon 2 were good places to grind for deathblows and money, respectively. And the Duneman Isles is supposed to be a gauntlet with the constant threat of dragons (which also have exclusive end game equipment on them). Not only would dropping the encounter rate here defeat the point of the optional dungeon, it’d be a lot harder to get exclusive weapons.

### Exp/gold:

- All exp/gold rates have been doubled.

As a way of compensating for lower encounter rates, money and experience have been doubled in this patch to keep the game from spiking in difficulty.


### Fast text:

old_script is for versions of the ROM where the script patch hasn't been applied. new_script is for versions of the ROM where the script patch has been applied. The old_script patch contains modified versions of specific scenes where the text scrolls too quickly. The new_script patch does not have these changes as the script patch provides these changes already.

The original patch for the fast text patch was created by FFMaster. As stated on the page where the original patch was provided, there are specific scenes where going through the text too quickly will crash the game. These are provided in detail in the link below:

https://ffhacktics.com/smf/index.php?topic=11638.0

#### Changed scenes in old_script:

- Shakhan's speech before the tournament.
- The UFO story in the Orphanage.
- The Ethos data banks.
- Bart's monologue about Ft Jasper when it is first used.

### Items/spells:

#### Field equipment changes:

Elly:
- Magical Rod: +4 to +5.
- Arcane Rod: +6 to +7.
- Hot Rod: +8 to +9.
- Lunar Rod: +8 to +12.
- Marshal Rod: +12 to +16.
- Thunder Rod: +14 to +19.
- Stone Rod: +14 to +19.
- Flare Rod: +16 to +19.
- Ice Rod: +14 to +19.
- Queen's Rod: +18 to +24.
- Dynamite Rod: +20 to +32.
- Dark Rod: +22 to +34.
- Chaos Rod: +24 to +31.
- Psycho Rod: +28 to +36.
- Skull Rod: +38 to +40.

Bart:
- WhippaSnappa: +6 to +8.
- CobraCracka: +5 to +9.
- Serpent Sting: +7 to +10, add 'Inflict Confusion on hit'
- Black Snake: +7 to +12, add 'Lower Ether Def' 
- Silver Blood: +8 to +14, add 'Inflict Silence on hit' 
- Whip-Lasher: +10 to +16, add 'Inflict Poison on hit' 
- Desert Worm: +12 to +18, add 'Inflict Confusion on hit'
- Sonic Wave: +14 to +24, add 'Lower Ether Def' 
- Wonder Whip: +16 to +28, add 'Inflict Silence on hit' 
- Thor’s Thunda: +20 to +34
- Death Adder: +24 to +30, Add ‘Inflict Stop on hit’

Barts whips having status effects are a really underused part of his kit. His whips in general were unimpressive. They don’t have the power of Billy’s guns or Citan’s swords, but unlike Elly he didn’t have as many unique utility weapons to compensate. They really just kept his attack comparable to Fei’s. Whips have been reworked with utility first in mind, with the idea of switching between them to be helpful for the situation at hand.

- Yamato Sword: Has an instant death effect.

The Yamoto Sword is the entire reason and main reward for going to the Duneman Isles. However, the blade the dragons drop here literally does more damage. This is hyper demotivating. So the instant death effect was given to make players not only feel more satisfied for getting this, it also gives you a more fun tool to play with. 

- Elly Jr Doll: +35 Ether rather than randomly inc/dec Ether power.
- Emeralda Jr Doll: Now has the same effect as Economether.
- Mermaid Ring: Provides Economether effect.
- Ether Doubler: Can be purchased at Shevat and Yggdrasil III (Disc 2)
- Slayer Robe: Can be purchased at Yggdrasil III (Disc 2)

#### Gear equipment changes:

- Serpent GWhip: Add 'Def Down' 
- BSnake GWhip: Add 'Power loss'
- SBlood GWhip: Add 'Camera damage'
- Thor GWhip: +100 to +150, effect moved to CCracka GWhip 
- Magnetic Coat: Can be purchased at Shevat and Yggdrasil III (Disc 2)
- Power Magic: Ether Amp +10 to +7. Can be purchased at the Anima Dungeons and Merkabah (first trip).

#### New items:

- Cross: Fei exclusive. +1 Ether/Ether Def. Same effect as Holy Pendant. Initial equip on Fei.
- NisanCross: Fei exclusive. +10 Ether/Ether Def. Same effect as Holy Pendant. Dropped by Ophiomorphous
- CCracka GWhip: +90 Attack and Inflicts Slow on hit. Available to buy at Tammuz after Gate 2 
- Lasher GWhip: +75 Attack and Inflicts Fuel Leak on hit. Available to buy in Nisan after Gate 1 
- Citan Jr Doll: Increases speed by 2. Available to buy in the Yggdrasil III in Disc 2 for 60000.
- Rico Jr Doll: Increases attack by 32. Available to buy in the Yggdrasil III in Disc 2 for 60000.
- Maria Jr Doll: Increases EtherDef by 40. Available to buy in the Yggdrasil III in Disc 2 for 60000.
- Margie Jr Doll: LifeStone and +5 to Eth/Edef. Available to buy in the Yggdrasil III in Disc 2 for 60000.

#### Item changes:

- Eyeballs, Fangs, Scales and Ivory have had their sale prices doubled. With encounters dropping, this change is done as a way to keep their value. This is particularly useful for Anima Dungeon 2 as if you’re revisiting the dungeon you’re certainly here because you need money for Big Joe's shop. 
- Speed Star now costs 2500.
- Samson's Hair can be bought at the Tamuz (first visit) for 1000.
- Omegasol now costs 150.
- Speed Ring now costs 3000.
- Ether Maxer is now sold by Hammer, Tamuz and Shevat's shops.
- Ether Reversa is now sold by Solaris and Crashed Shevat's shops. Its price has been increased to 20,000.
- V18-2000 is now V18-2500.
- V15-2000 is now V15-3000.
- All of Elly's elemental rods cost 2600.

#### Monster drops:

- The Executioner drops the ExecutionerGown.
- Alpha Weltall at Raziel now drops a Power Crisis. The Alpha Weltall fight is great. Its rewards, not so much. You need the Trader's Card just to get it, and it’s generic armor. Not even the best for pure defense. So at base for beating the boss you’ll receive a Power Crisis. This will make the few sections left on foot at least a little more enjoyable if you miss the first one or want more. 
- Ophiomorphous drops the NisanCross.

#### Spell changes:

Fei:
- Guided Shot: 20 to 40
- Radiance: 60 to 80
- Big Bang: 80 to 120

Fei’s generally pretty great when balanced against the cast. But against himself, his Ether attacks tend to be very overshadowed and down right wasteful to use. All options should have merit. Even if it’s situational, a situational merit is better than consistently being eclipsed by basic attacks. They’re viable with Power Magic abuse, but even Steel Mettle is with that. These changes will give players better incentive to use these moves throughout the game. It also helps Weltall a fair bit. It tends to fall off hard by disc 1’s end. Between Renmazuo, Vierge, Siebzhen, Crescens (by technicality) and the addition of Andvari to your squad, Fei really struggles to carry his weight. This will let him be better at contributing. Big Bang is admittedly a very reliable spell even without its new buffs. But it’s only learned before the final dungeon typically, so this will help it see a little more prominence in disc 2.

- ~~Iron Valor~~ Iron Focus: Accuracy Up
- Yang Power: Ether Up 
- Yin Power: Attack Up 

This will let Fei choose to be a magic focused or physical character to make using him more interesting instead of the binary choice of buffing his attack. 


Weltall/Weltall-2/Xenogears:
- Yang Power: Ether Up 
- Yin Power: Attack Up 

Defense buffs simply don’t work in gears. So their effects have been altered to function better. 


Vierge:
- AERODS: Power: 50 to 40. Fuel cost: 400 to 800

Elly is a peculiar character. She herself is fine, and her gear isn’t too impressive. But AERODs throws all that out the window, allowing her to one shot several of the game's bosses. While bosses are getting a buff, she hits 9999 way too early and easily because of Ether Doublers and Power Magics. So along with Power Magics in general being nerfed simply for being too beneficial for how easy they are to get; AERODs are also losing some points from its power output so that, while the classic boss slayer build will still be a thing, you must now commit more to it to get results. This should give bosses more of a presence in the first disc as Elly soundly obliterates the mid-game, and let other options shine for gears because AERODs becomes a crutch for many players. And given its... limited availability, this isn’t exactly healthy to goad players to dump all investment into Vierge only to lose it in disc 2.


Citan:
- Koga: Ether Defense Up
- Yamgia: Attack Up

Changed the properties of Citan's buffs to differentiate him from Fei more.


Heimdall/Fenrir:
- Koga: Ether Defense Up
- Yamiga: Attack Up


Bart:
- ~~White Cologne~~ Divvy Up: Next Ether affects all

Allows Bart to have useful part wide buffs in exchange for no healing. 


Billy:
- Goddess Eyes: Ether Up

Goddess Eyes now raises Ether as part of the theme of reworking defensive buffs. Billy was the basis for most balance changes, as he’s arguably the most well-rounded character. Outside of low survivability, Billy makes a case for being the best gear pilot and fighter on foot. But it’s not really for anything overtuned. His gear damage is just the best in the game with nothing else going for it, and his on foot builds are very flexible at the cost of sacrificing other strengths. Raising his DPS with the doubler but lowering his support use, making all of his Ether options phenomenal but crippling his defense, pure support love for weaker offense and defense etc. Outside of quality of life changes that needed to be done simply because they weren’t working, we didn’t want to touch Billy. He’s good as is. 


Rico:
- ~~Steel Body~~ Steel Focus: Accuracy Up
- ~~Steel Spirit~~ Steel Tempo: Haste, self-only

Rico’s spirit moves have been changed to help him out more; Steel Spirit was effectively useless due to Rico’s garbage Ether Defense, and Steel Body was literally useless because it just... didn’t work. Steel Focus and Steel Tempo are there to offset his defining weaknesses of speed and accuracy without breaking him as a character. 


Chu-Chu:
- Forest Dance (foot): Heals all allies
- Forest Dance (giant): Power: 50 to 70

Chu-Chu’s healing is the selling point of the character outside abusing Drives, but in reality its output is pitiful. Especially considering how you don’t want to drag out gear fights, and you’re sacrificing damage for survivability. Healing is now adjusted to better support your attackers and make her a real viable pick.


Maria:
- Graviton Gun: 30 EP to 15 EP

On foot; Maria is actually pretty good and fairly underrated, as novice players are often turned off by her low speed and no deathblows. Due to her insanely high damaging spells that run off Seibzhen’s Ether stats including Power Magics, Maria can be a fairly strong on foot character with a good understanding of the game's mechanics. 

But Graviton Gun is way too costly in spite of this. By end-game you’ll only be able to use it twice and with the doubler equipped without the Economether, you're only capable of using it once. Maria isn’t even available for many on foot sections, so this should encourage players to use her and experiment more with an ability they would normally never see. 

### Monster stats:

#### Monster stats:

Unless stated otherwise, the stat changed is HP.

- Earth Seraph: 5000 to 18000
- Fire Seraph: 3400 to 18000
- Fire Seraph 2: 15800 to 20000
- Heal Seraph: 2000 to 18000
- Sword Seraph: 2500 to 18000
- Sword Seraph 2: 3500 to 18000 
- Water Seraph: 1800 to 18000
- Water Seraph 2: 3400 to 18000			
- Wind Seraph: 3500 to 18000
- Power Seraph: 15800 to 20000
- Assassin 1: HP 290 to 450 EDef 10 to 60 
- Assassin 2: HP 260 to 400 EDef 20 to 70 
- Geburah Guard: 160 to 350
- Solaris Guard: 200 to 530 
- Gimmick: 6 to 12 
- Wels: 200 to 300 
- Neo Wels: 200 to 400 
- Dorothy: 300 to 400 
- Tin Robo: 138 to 250 
- Medusoid: 65 to 130
- Medusoid 2: 400 to 800 
- HarquebusMk10: EDef 30 to 60
- NeoMushaMk100: EDef 40 to 80 
- Hatamoto Mk3: EDef 40 to 80 
- Shinobi Mk0: EDef 40 to 80 
- Traffic Jam: 345 to 600 
- Gun Drone: 400 to 600 
- Dune Man: HP 200 to 1000 Atk 100 to 150 Agility 11 to 13 Hit 40 to 100 
- Hobgob (Sewers): 90 to 200 
- Armour Grub: 7 to 15
- Armour Wasp: 77 to 150 
- Abandon: 150 to 300 
- Shellbelle: 680 to 800 					
- Dive Bomber: 30 to 60 
- Lucre Bug: 77 to 150 
- Neo Gimmick: 7 to 14 

#### Monster weaknesses:

- Abandon is now weak to Poison 
- Wels are now weak to Confuse 

#### Boss stats:

- Swordknight: Atk 90 to 100. Def 40 to 30 
- Aegisknight: Atk 80 to 90. Def 60 to 50 
- Wandknight: Atk 90 to 100. Def 40 to 30
- Clawknight: Atk 80 to 90. Def 40 to 30
- Schpariel: Atk 90 to 100. Def 40 to 30. Hit 20 to 80
- Ramsus: Def 35 to 38
- Wiseman: XP 1 to 4000
- Hecht: 7500 to 9500 Atk 200 to 250 Def 70 to 100 Ether 25 to 50
- Fis.6 HP 6800 to 17000 Atk 204 to 250 Def 80 to 100 EthDef 80 to 90
- Haishao (Dominia): HP 2800 to 4000. Def 100 to 125. EthDef 40 to 100
- Bladegash: HP 3000 to 6000 Hit 30 to 80
- Haishao (Ramsus): Def 100 to 125. EthDef 50 to 100
- Bloody (Ethos Transport): HP 8888 Atk 75 to 100 
- Giant Wels: HP 7000 to 14000 Atk 300 to 350 EthDef 0 to 250 Hit 30 to 50
- Throne: HP 2500 to 3000 Atk 80 to 120
- Seraphita: HP 2000 to 2500 
- Id: 3000 to 6666
- Alkanshel: Hit 10 to 90
- Wyvern: HP 5000 to 28000 Atk 330 to 400 EthDef 120 to 150 Agility 5 to 9 Hit 40 to 60
- Seibzehn: HP 5500 to 18000 Atk 380 to 500 Def 300 to 250 EDef 40 to 150 Hit 30 to 70
- White Knights: HP 290 to 600 
- Achtzehn: HP 12000 to 19000 Atk 600 to 800 Def 300 to 400 Ether 20 to 35 EDef 0 to 200 
- Shakhan: HP 18000 to 28000 Ether 120 to 200 EthDef 100 to 200
- Bladegash 2: HP 4000 to 14000 Atk 500 to 400 Hit 30 to 80
- Marinebasher: HP 5000 to 15000 Def 300 to 200 EDef 100 to 300
- Skyghene: HP 5000 to 16000 Evade 35 to 55 Hit 30 to 90 
- Grandgrowl: HP 6000 to 18000 Def 350 to 100 Edef 50 to 350
- Crescens: EDef 100 to 1000 Hit 50 to 70
- Grahf (Solaris): Atk 180 to 200 Eth 60 to 150
- Vendetta: HP 12000 to 22000 EthDef 50 to 200 Hit 50 to 60
- Ft Hurricane HP 13000 to 40000 Agility 14 to 7
- Sufal Mass: HP 4800 to 20000 EDef 50 to 100
- Sufal: Agility 7 to 10 Eth 20 to 50 
- Dominia: 4000 to 20000
- Cherubina: 2500 to 15000
- Throne: HP 2500 to 18000 Eth 40 to 80
- Seraphita: 2500 to 17000
- G-Elements: HP 45000 to 60000 Def 600 to 300 EthDef 0 to 600 Hit 60 to 100
- Hammer: Hit 40 to 80
- Alpha Weltal: EthDef 100 to 800 Hit 50 to 100
- Ophiomorphous: HP 25000 to 30000 EthDef 50 to 500
- Id Xenogears: HP 30400 to 60000 Atk 825 to 1000 Ether 90 to 150 EthDef 100 to 200
- Alpha Weltall (Bethlehem): HP 15000 to 30000 Atk 650 to 800 Eth 100 to 150 EthDef 100 to 700 Hit 50 to 100
- Harlute: EthDef 0 to 255
- Metatron: EthDef 0 to 200
- Sundel: EthDef 0 to 800
- Marlute: EthDef 0 to 300
- Deus: HP 75000 to 150000 EthDef 0 to 700
- Ouroboros: HP 50000 to 75000

### Script:

Xenogears was translated from Japanese to English by one man in only 8 months. An incredibly respectable feat that kind of shows. Many words are mistranslated, spelling errors do appear, and a couple of rather infamously campy lines were put in like the iconic “My father? You mean my dad?” Line. The idea was to make the dialogue more faithful to Perfect Works and make the writing more refined to fit the story. Getting rid of a lot of dumb sounding lines is obviously something we were going to do to keep the tone as much as necessary, but an important thing is that we’re aware that Xenogears is still a video game. People are always going to find ways to laugh at things that weren’t intended to be funny. After all, you can name Chu-Chu and gears anything you want in the vanilla game. People will always find a way to take something less seriously. Even games known for being very dark stories such as Mother 3, Max Payne, and Fallout 1 all have moments that are unintentionally funny and fans of those games enjoy that aspect. 

So the plan is to keep some of the camp wherever it’s reasonable because the game is as memorable for funny moments as it is for dark moments. And those moments are important to fans. Not every comedic line is going to stay. The infamous father example being the most notable. But efforts were made to keep what ever ones didn’t take away from a scene. The game has a lot of iconic lines, like Id’s dropping a warship is cheating line that fans would be turned away if they changed. So please don’t be too worried or concerned about localization changes. Fans were taken into consideration. 

#### Screenshots:

<a><img src="/screenshots/screenshot01.png" align="centre" height="194" width="263"></a>
<a><img src="/screenshots/screenshot02.png" align="centre" height="194" width="263"></a>

<a><img src="/screenshots/screenshot05.png" align="centre" height="198" width="263"></a>
<a><img src="/screenshots/screenshot06.png" align="centre" height="198" width="263"></a>

<a><img src="/screenshots/screenshot03.png" align="centre" height="194" width="263"></a>
<a><img src="/screenshots/screenshot04.png" align="centre" height="194" width="263"></a>

<a><img src="/screenshots/screenshot11.png" align="centre" height="194" width="263"></a>
<a><img src="/screenshots/screenshot12.png" align="centre" height="194" width="263"></a>

<a><img src="/screenshots/screenshot09.png" align="centre" height="148" width="263"></a>
<a><img src="/screenshots/screenshot08.png" align="centre" height="148" width="263"></a>

<a><img src="/screenshots/screenshot07.png" align="centre" height="194" width="263"></a>
<a><img src="/screenshots/screenshot10.png" align="centre" height="194" width="263"></a>




#### Name changes:

- True Weltall = Alpha Weltall
- Alkanshel = Arc-en-Ciel
- Anima Relics = Vessels of Anima
- Anonelbe = Ahnenerbe
- Aveh = Av
- Bledavik = Breidablik
- Broyer = Breuer
- Day of Destruction = Day of Collapse
- Elhaym = Elehayym
- Elru = Elul
- Ethos = The Church/Church of Ethos
- Etrenank = Etemenanki
- Gaetia Key = Key of Goetia
- Gebler = Geburah
- Golgada = Golgotha
- Kadomony = Kadamoni 
- Heimdal = Heimdall 
- Helmholz = Helmholtz
- Kahran = Carlin (and Kahr to Carl)
- Kelvena = Cherubina
- Mahanon = Machanon
- Merkava = Merkabah
- Miang = Myyah
- Nikolai = Nikola
- Nortune = Noatun
- Opiomorph = Ophiomorphous
- Razael = Raziel
- Renk = Rank
- Ricdeau = Rikudou
- Taura = Torah
- Thames = Tamuz
- Tolone = Throne
- Urobolus = Ouroboros
- Yabeh = YHWH
- Wind Mode = Zephyr Mode
- Earth Mode = Seismic Mode
- Fire Mode = Inferno Mode
- Water Mode = Torrent Mode
- Bart Missiles = Bart Bombs 
- Vance = Franz 
- Franz = Vance 
- Amphysvena = Amphisbaena 
- Harlute	= Harut 
- Marlute =	Marut
- Sundel = Sandalphon
- Schpariel	= Spirale 
- Seibzehn = Siebzehn 
- Skyghene = Skyscreech 
- Sufal	= Sphar
- Suzarn = Suzanne 
- Krelian = Karellen
- Stratski = Strachey

#### Story changes:

- Liquid Sky is referred to by Jessie and Sigurd in the gunroom and by Maison after Shakhan.
- The names of the Gazel Ministry are referenced by Karellen in their final scene.

#### Chapter names:

- Path to Av > Path to Aveh
- Operation Av > Operation Aveh
- Recapture Av > Recapture Aveh
- Merkabah Calls > Merkava Calls

#### Music changes:

- Where Dreams Hatch is used when Elly and Citan talk in Blackmoon Forest over Earthly Tears, Mortal Thoughts.
- Premonition is no longer used when the group leave Nisan.
- In a Dark Slumber is no longer used when the Yggdrasil retreats in the desert.
- The Battle Arena now plays Infiltration over Deadly Dance.
- Blue Traveller continues playing against Giant Wels.
- Fuse plays when Chu-Chu transforms over Soaring.
- Blue Traveller plays when Ft Jasper emerges over Soaring.
- Fuse now plays when the Elements merge over Soaring.
- Earthly Tears, Mortal Thoughts plays during Dan's disc 2 scene over October Mermaid.
- Blue Traveller plays in Big Joe's shop over Soaring.

### Stat changes:

Xenogears is frankly; terribly balanced. Not exactly its fault in particular, Squaresoft games were all infamous for this, even their well-balanced games like Chrono Trigger had hiccups. Most of the cast has been rebalanced primarily around Billy and Emeralda as they’re frankly the most balanced party members in the game across gears and on-foot. Buffs are the priority! There are some necessary nerfs we begrudgingly made, but we really wanted to focus on buffing the roster. The issue with the roster's balance, primarily a bunch of not useful options and characters, was where the focus was. Characters and gears will be discussed in the same sections. 

Elly:
- Hit 7 to 10
- Ether 15 to 20

To help offset this and due to us not being a fan of strictly nerfing characters without compensations to other parts of their kit, Elly is receiving buffs for on-foot that will make her more inviting of an option. Though she is a mandatory party member fairly often, this is in gear sections a majority of the time. So this will hopefully make her a bit more flexible overall. 


Bart:

- Defense 10 to 14
- Ether 7 to 14
- Evade 14 to 16

Bart is generally one of the weakest characters in the game due to mediocrity. For most of the game, anything he can do others can do better. Wild Smile is his biggest selling point, though its effects can be replicated with an Evasion Ring. The goal was to rework Bart to be much more supportive and improve his jack-of-all-trades nature to be more useful for him. 

These stat changes should allow Bart to be a lot more useful overall, without just buffing his attack stat to compensate for his middling toolkit. It also helps give him a sense of identity on the roster, as well as using generally underutilized stats in the game. No one in particular focused on Defense or Evasion, so efforts are being made to help these stats play more of an impact as well as diversify the party. 


Rico:
- HP 110 to 200
- Attack 28 to 60
- Defense 23 to 38
- Hit 10 to 18
- Agility 7 to 8

Rico is iconically bad. He's tied for lowest speed in the game. His Agility and Hit stats are obviously bad, but he doesn’t have anything to compensate for this. When he joins, most of his stats are worse than Fei’s. Which really undersells how he’s one of the few people who could beat Fei in a 1v1 fight. Combined with no utility like healing or area of effect damage and Rico is comedically undercooked. The easy solution would be just to give him high speed, but that just ruins the diversity of the cast. If that’s what we changed, people would still not pick Rico because he’s just a worse Citan. We decided to double down on making Rico a juggernaut. His stats and growths have been adjusted to make him an offensive behemoth, so he has noticeable strengths. We also slightly raised his agility to match the boss fight and to prevent the Ganondorf conundrum of giving a character so much power and having them still not be good because they don’t get to use it. Plus, this ties him with Elly’s old agility stat, instead of putting him with the characters who lack deathblows. The same goes for his Hit rate, as his accuracy is actually hilariously good for his boss fight. The balance for Rico’s absurd stats is his complete lack of any utility (no healing, cures, AOE, elemental coverage etc).


Emeralda:
- Agility: 12 to 14


Chu-Chu:
- Ether 12 to 25


Weltall/Weltall-2/Xenogears:
- Ether amp 10 to 15


Stier:
- Attack modifier: 12 to 14
- Agility 7 to 8

Stier was given a point of agility to help differentiate it from Seibzehn. They have very similar builds, which led to Stier being outclassed by Seibzehn once Power Magics came into play. This gives distinctions to similar characters. 


Seibzehn:
- Agility 8 to 6


El-Renmazuo:
- RENM-24800 24800 to 28000


El-Stier:
- Attack modifier: 12 to 14
- Agility 7 to 9


Yggdrasil IV:
- Agility 8 to 10


All deathblow learning rates (sans Emeralda) have been doubled to minimize grinding and in case the encounters patch has been applied. Elly's has been changed to 8 from 3 so that players have more time to get her deathblows for plot related reasons.

## Glitches:

### General:
- Items/spells and exp/gold together can cause unpredictable effects in battle. Will be fixed in 0.1.2. Can provide an easy way to access the kernel when playing on hardware.

### Fast text:
- Going through Ramsus' introductory scene too quickly will freeze the game (legacy glitch from the original patch).

### Items/spells:
- If Divvy Up is used immediately after it's been used, it's effect will stack onto other party members (will not affect the rest of the game).

### Script:
- Texture issue with Fatima Castle's library (will not affect the rest of the game). To be fixed.


## Links:

Join our discord if you have any issues or you want to communicate with us:
https://discord.gg/j4v6yCcWRK

Provide feedback here:
https://docs.google.com/forms/d/1KZAwqR70pABGXtuR7l8GVMjY487WKOn6Qrb3fUHHumw/viewform?edit_requested=true

DM or tag here if you want immediate feedback on Twitter:
@NoharOS

## Credits:

### Translation: 
- An Eye for the Infinite http://moonreading.lunarpages.com/xeno/
- Almagest http://almagest712.blogspot.com/2017/11/xenogears-my-father-you-mean-my-dad.html
- Ryan Piccuito ↙ ➡ ↘ ⬇ ↙ ⬅ ↘
- rafael
- ribbiwawa
- Will Bill
- edrovinhaz
- Xenocritus
- cactusmomma
- krmine
- Nohar

### Character balance:
- Ryan Piccuito ↙ ➡ ↘ ⬇ ↙ ⬅ ↘
- rafael
- Will Bill
- Xenocritus

### Programming:
- rufaswan
- rulv
- Will Bill
- Xenocritus
- krmine
- edrovinhaz
- Nohar

### Testing:
- CosmicFrame
- Ryan Piccuito ↙ ➡ ↘ ⬇ ↙ ⬅ ↘
- rafael
- The Minester
- cthuljew
- RayDreamer
- ChaoticBrave
- Aero JP
- Nohar

### Tools:
- SadNES cITy Translations

### Fast text code:
- FFMaster
- Nohar

### PPF code:
- meunierd
