# \# Verse Test - AI Vehicle Bot

# 

# Technical assessment implementation for the Verse AI Vehicle programming challenge.

# 

# \*\*Engine:\*\* Unreal Engine 5.6  

# \*\*Language:\*\* C++  

# \*\*Networking:\*\* Server-Authoritative Multiplayer  

# \*\*Physics:\*\* Chaos Vehicles

# 

# \---

# 

# \## Project Structure

# 

# ```text

# Source/

# ├── VehicleBotComponent

# │   - AI navigation

# │   - Path following

# │   - Steering

# │   - Throttle control

# │   - Stuck detection

# │   - Recovery

# │   - Runtime diagnostics

# │

# └── VerseTestPawn

# &#x20;   - Chaos Vehicle Pawn

# 

# Content/

# ├── VehicleBasic Map

# ├── Waypoints

# └── Navigation Mesh

# ```

# 

# \---

# 

# \## Overview

# 

# This project implements an autonomous AI-controlled Chaos Vehicle for the Verse technical assessment.

# 

# The bot navigates through waypoint checkpoints using Unreal Engine's Navigation System, follows generated navigation paths, dynamically adjusts steering and throttle, detects when it becomes stuck, performs recovery maneuvers, and supports multiplayer through a server-authoritative architecture.

# 

# \---

# 

# \## Features

# 

# \### Vehicle Navigation

# 

# \- Automatic waypoint collection at runtime.

# \- Dynamic NavMesh path generation.

# \- Automatic path rebuilding when reaching the next waypoint.

# \- Smooth steering interpolation.

# \- Adaptive throttle based on steering angle.

# \- Continuous waypoint patrol.

# 

# \### Stuck Detection \& Recovery

# 

# \- Detects when the vehicle is no longer making progress.

# \- Waits for a configurable timeout before triggering recovery.

# \- Performs a reverse recovery maneuver.

# \- Automatically rebuilds the navigation path after recovery.

# 

# \### Runtime Diagnostics

# 

# Diagnostics can be enabled during gameplay using:

# 

# ```text

# yourbot.Diag 1

# ```

# 

# Disable diagnostics:

# 

# ```text

# yourbot.Diag 0

# ```

# 

# The diagnostics output includes:

# 

# \- Vehicle speed

# \- Current waypoint

# \- Current AI state

# \- No-progress reason

# 

# Example:

# 

# ```text

# \[BOT] Speed=1580 cm/s | Waypoint=3 | State=Driving | Reason=None

# ```

# 

# \---

# 

# \## Multiplayer

# 

# The AI is fully server-authoritative.

# 

# \- AI logic executes only on the server.

# \- Vehicle movement is replicated to clients.

# \- Tested using a Listen Server with replicated clients.

# \- Clients receive replicated movement while all AI decisions remain server-side.

# 

# \---

# 

# \## Configurable Parameters

# 

# The following gameplay values are exposed in the Unreal Editor.

# 

# \### Driving

# 

# \- MaxThrottle

# \- MinThrottle

# \- SteeringInterpSpeed

# \- ThrottleInterpSpeed

# 

# \### Recovery

# 

# \- StuckTimeThreshold

# \- MinMovementSpeed

# \- RecoveryDuration

# \- RecoveryImpulse

# 

# \### Diagnostics

# 

# \- DiagnosticInterval

# 

# \---

# 

# \## Measurement Results

# 

# | Metric | Result |

# |---------|--------|

# | Test Duration | 5 minutes |

# | Waypoints Reached | 4 / 7 |

# | Recovery Events | 6 |

# | Unrecovered Failure Cases | 2 |

# 

# \---

# 

# \## Test Observations

# 

# The AI vehicle was tested for five minutes in a multiplayer Listen Server session.

# 

# During the test, the vehicle successfully reached \*\*4 out of 7 waypoints\*\* and triggered the recovery system \*\*6 times\*\*.

# 

# Two unrecovered failure scenarios were observed:

# 

# \- The vehicle became stuck between walls due to an invalid navigation path and was unable to recover automatically.

# \- The vehicle rolled onto its side against a wall, causing the current recovery logic to continuously apply reverse input without restoring the vehicle to a drivable state.

# 

# \---

# 

# \## Known Limitations

# 

# \- When the next waypoint is positioned behind the vehicle, the steering logic rotates the vehicle until it directly faces the target instead of selecting a shorter turning maneuver.

# \- Recovery currently relies on a simple reverse maneuver and does not evaluate nearby obstacles before choosing an escape direction.

# \- The current recovery system cannot recover from vehicle rollover scenarios.

# \- Navigation depends on a valid NavMesh path. If the generated path becomes invalid or leads into blocked geometry, the bot may remain stuck until a new valid path can be generated.

# 

# \---

# 

# \## Future Improvements

# 

# Given additional development time, I would:

# 

# \- Improve local obstacle avoidance.

# \- Implement predictive steering instead of directly targeting the next path point.

# \- Improve waypoint handling when the next waypoint is positioned behind the vehicle.

# \- Implement multiple recovery strategies based on the detected failure type.

# \- Add rollover detection with an automatic self-righting system.

# \- Extend the diagnostics system with on-screen debugging and additional runtime metrics.

# 

# \---

# 

# \## Build Instructions

# 

# 1\. Clone the repository.

# 2\. Open the project using \*\*Unreal Engine 5.6\*\*.

# 3\. Generate Visual Studio project files if necessary.

# 4\. Build the project.

# 5\. Open the \*\*VehicleBasic\*\* map.

# 6\. Press \*\*Play\*\*.

# 

# To enable runtime diagnostics:

# 

# ```text

# yourbot.Diag 1

# ```

# 

# To disable runtime diagnostics:

# 

# ```text

# yourbot.Diag 0

# ```

