#ifndef OPENSIM_FREE_JOINT_H_
#define OPENSIM_FREE_JOINT_H_
/* -------------------------------------------------------------------------- *
 *                           OpenSim:  FreeJoint.h                            *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2017 Stanford University and the Authors                *
 * Author(s): Ajay Seth                                                       *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

// INCLUDE
#include "Joint.h"

namespace OpenSim {

//=============================================================================
//=============================================================================
/**

A class implementing a Free joint.  The underlying implementation
in Simbody is a SimTK::MobilizedBody::Free.
Free joint allows unrestricted motion with three rotations and three translations.
Rotations are modeled similarly to BallJoint -using quaternions with no
singularities- while the translational generalized coordinates are XYZ
Translations along the parent axis. Generalized speeds are equal to the computed
angular velocities (\f$\vec{u} = \vec{\omega}\f$), not a differentiation of
position (\f$\vec{u} \neq \dot{\vec{q}}\f$).

\image html freeJoint.gif

@author Ajay Seth
*/

class OSIMSIMULATION_API FreeJoint : public Joint {
OpenSim_DECLARE_CONCRETE_OBJECT(FreeJoint, Joint);

public:
    /** Indices of Coordinates for use as arguments to getCoordinate() and
        updCoordinate().

        <b>C++ example</b>
        \code{.cpp}
        const auto& rx = myFreeJoint.getCoordinate(FreeJoint::Coord::Rotation1X);
        \endcode

        <b>Python example</b>
        \code{.py}
        import opensim
        rx = myFreeJoint.getCoordinate(opensim.FreeJoint.Coord_Rotation1X)
        \endcode

        <b>Java example</b>
        \code{.java}
        rx = myFreeJoint.getCoordinate(FreeJoint.Coord.Rotation1X);
        \endcode

        <b>MATLAB example</b>
        \code{.m}
        rx = myFreeJoint.get_coordinates(0);
        \endcode
    */
    enum class Coord: unsigned {
        Rotation1X   = 0u, ///< 0
        Rotation2Y   = 1u, ///< 1
        Rotation3Z   = 2u, ///< 2
        TranslationX = 3u, ///< 3
        TranslationY = 4u, ///< 4
        TranslationZ = 5u  ///< 5
    };

private:
    /** Specify the Coordinates of the FreeJoint */
    CoordinateIndex rx{ constructCoordinate(Coordinate::MotionType::Rotational,
                                   static_cast<unsigned>(Coord::Rotation1X)) };
    CoordinateIndex ry{ constructCoordinate(Coordinate::MotionType::Rotational,
                                   static_cast<unsigned>(Coord::Rotation2Y)) };
    CoordinateIndex rz{ constructCoordinate(Coordinate::MotionType::Rotational,
                                   static_cast<unsigned>(Coord::Rotation3Z)) };
    CoordinateIndex tx{ constructCoordinate(Coordinate::MotionType::Translational,
                                   static_cast<unsigned>(Coord::TranslationX)) };
    CoordinateIndex ty{ constructCoordinate(Coordinate::MotionType::Translational,
                                   static_cast<unsigned>(Coord::TranslationY)) };
    CoordinateIndex tz{ constructCoordinate(Coordinate::MotionType::Translational,
                                   static_cast<unsigned>(Coord::TranslationZ)) };

public:
    /** Use Joint's constructors. @see Joint */
    using Joint::Joint;

    /** Exposes getCoordinate() method defined in base class (overloaded below).
        @see Joint */
    using Joint::getCoordinate;

    /** Exposes updCoordinate() method defined in base class (overloaded below).
        @see Joint */
    using Joint::updCoordinate;

    /** Get a const reference to a Coordinate associated with this Joint.
        @see Coord */
    const Coordinate& getCoordinate(Coord idx) const {
        return get_coordinates( static_cast<unsigned>(idx) );
    }

    /** Get a writable reference to a Coordinate associated with this Joint.
        @see Coord */
    Coordinate& updCoordinate(Coord idx) {
        return upd_coordinates( static_cast<unsigned>(idx) );
    }

protected:
    // ModelComponent interface.
    void extendAddToSystem(SimTK::MultibodySystem& system) const override;
    void extendInitStateFromProperties(SimTK::State& s) const override;
    void extendSetPropertiesFromState(const SimTK::State& state) override;

private:
    SimTK::ResetOnCopy<SimTK::MobilizedBodyIndex> _masslessBodyIndex;
    void setNull();
//=============================================================================
};  // END of class FreeJoint
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // OPENSIM_FREE_JOINT_H_
