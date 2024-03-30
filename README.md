# Comprehensive Dismemberment System in Unreal Engine 5

This project showcases a comprehensive dismemberment system implemented in Unreal Engine 5 (UE5) using C++. The system is seamlessly integrated with Unreal Engine's FPS template, enhancing the gameplay experience by introducing realistic and dynamic character dismemberment.

## Core Mechanics

The core mechanic of the dismemberment system revolves around assigning individual health values to each bone in the character's skeletal mesh. When a bone is hit, its health decreases accordingly. Once the health of a specific bone reaches zero, the "Dismemberment" function is triggered, breaking the constraints that connect the bone to the main body, effectively disassembling it.

## Realistic Physics Simulation

To add an extra layer of realism, physics simulation is applied to the disassembled bones, allowing them to interact with the environment and move around in a realistic manner. This creates a visually striking and immersive experience as the dismembered body parts react to the game world.

## Zombie-like Behavior

One of the unique features of this dismemberment system is the ability to create a zombie-like behavior when one of the character's spine bones is broken. In such cases, the character will start crawling towards the player, mimicking the behavior of a zombie. This adds an interesting gameplay element and increases the overall intensity of the experience.

## Implementation Details

The project leverages the power of C++ to ensure optimal performance and flexibility, while seamlessly integrating with Unreal Engine 5's features and workflows. The code is well-structured and documented, making it easy for other developers to understand and build upon.

## Pushing Boundaries

By combining realistic dismemberment mechanics with engaging gameplay elements, this project aims to push the boundaries of what's possible in terms of character interaction and destruction in video games. It serves as a solid foundation for developers looking to incorporate advanced dismemberment systems into their own projects.

## Future Enhancements

- Integration with AI systems to create more dynamic and responsive enemy behaviors based on their dismemberment state.
- Optimization of the dismemberment system to support a larger number of characters on screen simultaneously.
- Addition of visual effects, such as blood splatter and particle effects, to enhance the visual impact of dismemberment.
- Expansion of the system to support different types of characters and creatures with unique dismemberment patterns.

## Getting Started

To get started with this project, follow these steps:

1. Clone the repository to your local machine.
2. Open the project in Unreal Engine 5.
3. Compile the C++ code and ensure all dependencies are properly linked.
4. Run the project and experiment with the dismemberment system in action.

Feel free to explore the code, make modifications, and adapt it to suit your specific needs. We welcome contributions and feedback from the community to further improve and expand the capabilities of this dismemberment system.
