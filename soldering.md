# Soldering Reference

For the soldering lab, you'll be assembling these LED array board. These contain 8 LEDs, each connected to a header pin on one side and a resistor to a common ground on the other side. These boards are meant to be a debugging tool - plug into a breadboard, and you have an instant bank of LEDs to display various signals.

| Front side - LED side | Back side - resistor side |
| - | - |
| ![Image](ledarray/render_front.png?raw=true) | ![Image](ledarray/render_back.png?raw=true) |
| Left 8 components are LEDs, the rightmost component is a 0 ohm resistor | Leftmost component is a 0 ohm resistor, the right 8 components are 1k resistors |

<details>
  <summary>View without soldermask - showing internal copper connections</summary>

  | Front side | Back side |
  | - | - |
  | ![Image](ledarray/render_front_nomask.png?raw=true) | ![Image](ledarray/render_back_nomask.png?raw=true) |

</details>

PCB files (KiCad) are avaialble in the [ledarray folder](ledarray).

## Soldering Technique

We will demo the technique according to this excellent [SMT soldering manga](https://docs.google.com/file/d/0B1Q2XTPf2Q8xRjNiU3hGeVhER0E/edit?pli=1&resourcekey=0-x48EKSmnAKT_BzBhRj0Xvw), which we encourage you to keep open as a reference.

Misc soldering tips:
- **Solder only adheres to hot surfaces**. Both the component lead / pin and board pad must be hot to form a good joint. The iron tip must be touching both surfaces to heat them up.
   - Recommendation: when feeding solder wire directly into the joint, feed it to the component lead / pin or board pad, instead of onto the iron tip.
- **Apply flux whenever reheating solder**. Flux cleans oxidation, which impedes good joints. There is flux in the solder wire, but that is used up when heated the first time. If you're reheating a solder joint, apply flux to the joint.
- **LEDs are polarized devices**, with an anode (positive) and cathode (negative) terminal. The cathode terminal is commonly marked with a green dot. You can also test the LED with a multimeter on diode mode.
- **Soldering is about maximizing heat transfer from the iron to the board**. This can be done by:
  - **Keep the iron tip tinned and clean**. A tinned (coated with a thin layer of solder) tip conducts heat better. However, this coating oxidizes at high (eg, soldering) temperatures, so is sacrificial and must be cleaned regularly by wiping on the brass sponge, then re-coating with fresh solder.
    - If it looks dull (instead of shiny) or dark grey, you may want to clean and re-tin the tip
  - **Use the broad side of the iron tip**. Thermal transfer is a function of contact area, which you want to maximize.
    - If using a chisel / screwdriver tip (recommended), use the edge.
    - If using a conical tip (not recommended), use the broadside (side of the come) instead of the point tip.
- **Use tools to prevent the board from moving**, such as taping it down.
