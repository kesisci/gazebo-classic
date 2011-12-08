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

#include "physics/Collision.hh"
#include "physics/PlaneShape.hh"

using namespace gazebo;
using namespace physics;


////////////////////////////////////////////////////////////////////////////////
/// Constructor
PlaneShape::PlaneShape(CollisionPtr parent) 
  : Shape(parent)
{
  this->AddType(PLANE_SHAPE);
  this->SetName("plane_shape");
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor
PlaneShape::~PlaneShape()
{
}

////////////////////////////////////////////////////////////////////////////////
/// Load the plane
void PlaneShape::Load( sdf::ElementPtr &_sdf )
{
  Shape::Load(_sdf);
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize the plane
void PlaneShape::Init()
{
  this->CreatePlane();
}

////////////////////////////////////////////////////////////////////////////////
/// Create the plane
void PlaneShape::CreatePlane()
{
}

////////////////////////////////////////////////////////////////////////////////
/// Set the altitude of the plane
void PlaneShape::SetAltitude(const math::Vector3 &/*_pos*/) 
{
}

////////////////////////////////////////////////////////////////////////////////
/// Set the normal
void PlaneShape::SetNormal( const math::Vector3 &norm )
{
  this->sdf->GetAttribute("normal")->Set(norm);
  this->CreatePlane();
}

math::Vector3 PlaneShape::GetNormal() const
{
  return this->sdf->GetValueVector3("normal");
}

void PlaneShape::FillShapeMsg(msgs::Geometry &_msg)
{
  _msg.set_type(msgs::Geometry::PLANE);
  msgs::Set(_msg.mutable_plane()->mutable_normal(), this->GetNormal());
}

void PlaneShape::ProcessMsg(const msgs::Geometry &_msg)
{
  this->SetNormal(msgs::Convert(_msg.plane().normal()));
}