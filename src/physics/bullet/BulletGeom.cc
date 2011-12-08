/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
/* Desc: BulletCollision class
 * Author: Nate Koenig
 * Date: 13 Feb 2006
 * SVN: $Id:$
 */

#include <sstream>

#include "Mass.hh"
#include "PhysicsEngine.hh"
#include "BulletPhysics.hh"
#include "rendering/Visual.hh"
#include "common/Console.hh"
#include "World.hh"
#include "BulletLink.hh"

#include "BulletCollision.hh"

using namespace gazebo;
using namespace physics;

using namespace physics;

using namespace physics;


////////////////////////////////////////////////////////////////////////////////
// Constructor
BulletCollision::BulletCollision( Link *_body )
    : Collision(_body)
{
  this->SetName("Bullet Collision");
  this->bulletPhysics = dynamic_cast<BulletPhysics*>(this->physicsEngine);
  this->collisionShape = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
BulletCollision::~BulletCollision()
{
  if (this->collisionShape)
    delete this->collisionShape;
  this->collisionShape = NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// Load the collision
void BulletCollision::Load(common::XMLConfigNode *node)
{
  Collision::Load(node);
//  this->visualNode->SetPose( this->GetRelativePose() );
}

////////////////////////////////////////////////////////////////////////////////
// Save the body based on our common::XMLConfig node
void BulletCollision::Save(std::string &prefix, std::ostream &stream)
{
  Collision::Save(prefix, stream);
}

////////////////////////////////////////////////////////////////////////////////
// Update
void BulletCollision::Update()
{
  Collision::Update();
}

////////////////////////////////////////////////////////////////////////////////
// On pose change
void BulletCollision::OnPoseChange()
{
  math::Pose pose = this->GetRelativePose();
  BulletLink *bbody = (BulletLink*)(this->body);

  bbody->SetCollisionRelativePose(this, pose);
}

////////////////////////////////////////////////////////////////////////////////
/// Set the category bits, used during collision detection
void BulletCollision::SetCategoryBits(unsigned int bits)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Set the collide bits, used during collision detection
void BulletCollision::SetCollideBits(unsigned int bits)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Get the mass of the collision
Mass BulletCollision::GetLinkMassMatrix()
{
  Mass result;
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// Get the bounding box, defined by the physics engine
void BulletCollision::GetBoundingBox( math::Vector3 &min, math::Vector3 &max ) const
{
  if (this->collisionShape)
  {
    btmath::Vector3 btMin, btMax;
    this->collisionShape->getAabb( btTransform::getIdentity(), btMin, btMax );

    min.Set(btMin.x(), btMin.y(), btMin.z());
    max.Set(btMax.x(), btMax.y(), btMax.z());
  }
}

////////////////////////////////////////////////////////////////////////////////
// Set the collision shape
void BulletCollision::SetCollisionShape( btCollisionShape *shape ) 
{
  this->collisionShape = shape;

  /*btmath::Vector3 vec;
  this->collisionShape->calculateLocalInertia(this->mass.GetAsDouble(), vec);
  */

  this->mass.SetCoG(this->GetRelativePose().pos);
}

////////////////////////////////////////////////////////////////////////////////
/// Get the bullet collision shape
btCollisionShape *BulletCollision::GetCollisionShape() const
{
  return this->collisionShape;
}

////////////////////////////////////////////////////////////////////////////////
// Set the index of the compound shape
void BulletCollision::SetCompoundShapeIndex( int index )
{
  this->compoundShapeIndex = 0;
}