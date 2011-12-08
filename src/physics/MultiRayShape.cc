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
#include "msgs/msgs.h"
#include "physics/MultiRayShape.hh"

using namespace gazebo;
using namespace physics;


////////////////////////////////////////////////////////////////////////////////
/// Constructor
MultiRayShape::MultiRayShape(CollisionPtr parent) 
  : Shape(parent)
{
  this->AddType(MULTIRAY_SHAPE);
  this->SetName("multiray");
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor
MultiRayShape::~MultiRayShape()
{
  this->rays.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Load a multi-ray shape from xml file
void MultiRayShape::Load( sdf::ElementPtr &_sdf)
{
  Shape::Load(_sdf);
}

////////////////////////////////////////////////////////////////////////////////
/// Init the shape 
void MultiRayShape::Init()
{
  math::Vector3 start, end, axis;
  double yawAngle, pitchAngle; 
  double yDiff;
  double horzMinAngle, horzMaxAngle;
  int horzSamples = 1;
  double horzResolution = 1.0;

  double pDiff = 0;
  int vertSamples = 1;
  double vertResolution = 1.0;
  double vertMinAngle = 0;
  double vertMaxAngle = 0;

  double minRange, maxRange;

  this->rayElem = this->sdf->GetElement("ray");
  this->scanElem = this->rayElem->GetElement("scan");
  this->horzElem = this->scanElem->GetElement("horizontal");
  this->vertElem = this->scanElem->GetElement("vertical");
  this->rangeElem = this->rayElem->GetElement("range");

  if (this->vertElem)
  {
    vertMinAngle = this->vertElem->GetValueDouble("min_angle");
    vertMaxAngle = this->vertElem->GetValueDouble("max_angle");
    vertSamples = this->vertElem->GetValueUInt("samples");
    vertResolution = this->vertElem->GetValueDouble("resolution");
    pDiff = vertMaxAngle - vertMinAngle;
  }

  horzMinAngle = this->horzElem->GetValueDouble("min_angle");
  horzMaxAngle = this->horzElem->GetValueDouble("max_angle");
  horzSamples = this->horzElem->GetValueUInt("samples");
  horzResolution = this->horzElem->GetValueDouble("resolution");
  yDiff = horzMaxAngle - horzMinAngle;

  minRange = this->rangeElem->GetValueDouble("min");
  maxRange = this->rangeElem->GetValueDouble("max");

  this->offset = this->collisionParent->GetRelativePose().pos;

  // Create and array of ray collisions
  for (unsigned int j = 0; j < (unsigned int)vertSamples; j++)
  {
    for (unsigned int i = 0; i < (unsigned int)horzSamples; i++)
    {
      yawAngle = (horzSamples == 1) ? 0 : 
        i * yDiff / (horzSamples - 1) + horzMinAngle;

      pitchAngle = (vertSamples == 1)? 0 :  
        j * pDiff / (vertSamples - 1) + vertMinAngle;

      axis.Set(cos(pitchAngle) * cos(yawAngle), 
               sin(yawAngle), sin(pitchAngle)* cos(yawAngle));

      start = (axis * minRange) + this->offset;
      end = (axis * maxRange) + this->offset;

      this->AddRay(start,end);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// Get detected range for a ray.
/// \returns Returns DBL_MAX for no detection.
double MultiRayShape::GetRange(int index)
{
  if (index < 0 || index >= (int)this->rays.size())
  {
    std::ostringstream stream;
    stream << "index[" << index << "] out of range[0-"
      << this->rays.size() << "]";
    gzthrow(stream.str());
  }

  return this->rays[index]->GetLength();
}

////////////////////////////////////////////////////////////////////////////////
/// Get detected retro (intensity) value for a ray.
double MultiRayShape::GetRetro(int index)
{
  if (index < 0 || index >= (int)this->rays.size())
  {
    std::ostringstream stream;
    stream << "index[" << index << "] out of range[0-"
      << this->rays.size() << "]";
    gzthrow(stream.str());
  }

  return this->rays[index]->GetRetro();
}

////////////////////////////////////////////////////////////////////////////////
/// Get detected fiducial value for a ray.
int MultiRayShape::GetFiducial(int index)
{
  if (index < 0 || index >= (int)this->rays.size())
  {
    std::ostringstream stream;
    stream << "index[" << index << "] out of range[0-"
      << this->rays.size() << "]";
    gzthrow(stream.str());
  }

  return this->rays[index]->GetFiducial();
}

////////////////////////////////////////////////////////////////////////////////
/// Update the collision
void MultiRayShape::Update()
{
  double maxRange = this->rangeElem->GetValueDouble("max");

  // Reset the ray lengths and mark the collisions as dirty (so they get
  // redrawn)
  unsigned int ray_size = this->rays.size();
  for (unsigned int i=0; i < ray_size; i++)
  {
    this->rays[i]->SetLength( maxRange );
    this->rays[i]->SetRetro( 0.0 );

    // Get the global points of the line
    this->rays[i]->Update();
  }

  this->UpdateRays();
}

////////////////////////////////////////////////////////////////////////////////
/// Add a ray to the collision
void MultiRayShape::AddRay(const math::Vector3 &/*_start*/, 
                           const math::Vector3 &/*_end*/ )
{
  //msgs::Vector3d *pt = NULL;

  //FIXME: need to lock this when spawning models with ray.
  //       This fails because RaySensor::laserShape->Update() is called before rays could be constructed.
}


//////////////////////////////////////////////////////////////////////////////
/// Get the minimum range
double MultiRayShape::GetMinRange() const
{
  return this->rangeElem->GetValueDouble("min");
}
//////////////////////////////////////////////////////////////////////////////
///  Get the maximum range
double MultiRayShape::GetMaxRange() const
{
  return this->rangeElem->GetValueDouble("max");
}
//////////////////////////////////////////////////////////////////////////////
///  Get the range resolution
double MultiRayShape::GetResRange() const
{
  return this->rangeElem->GetValueDouble("resolution");
}



//////////////////////////////////////////////////////////////////////////////
/// Get the sample count
int MultiRayShape::GetSampleCount() const
{
  return this->horzElem->GetValueUInt("samples");
}

//////////////////////////////////////////////////////////////////////////////
///  Get the range resolution
double MultiRayShape::GetScanResolution() const
{
  return this->horzElem->GetValueDouble("resolution");
}

//////////////////////////////////////////////////////////////////////////////
/// Get the minimum range
math::Angle MultiRayShape::GetMinAngle() const
{
  return this->horzElem->GetValueDouble("min_angle");
}

//////////////////////////////////////////////////////////////////////////////
///  Get the maximum range
math::Angle MultiRayShape::GetMaxAngle() const
{
  return this->horzElem->GetValueDouble("max_angle");
}





//////////////////////////////////////////////////////////////////////////////
/// Get the vertical sample count
int MultiRayShape::GetVerticalSampleCount() const
{
  return this->vertElem->GetValueUInt("samples");
}

//////////////////////////////////////////////////////////////////////////////
///  Get the vertical range resolution
double MultiRayShape::GetVerticalScanResolution() const
{
  return this->vertElem->GetValueDouble("resolution");
}

//////////////////////////////////////////////////////////////////////////////
/// Get the vertical minimum range
math::Angle MultiRayShape::GetVerticalMinAngle() const
{
  return this->vertElem->GetValueDouble("min_angle");
}

//////////////////////////////////////////////////////////////////////////////
///  Get the vertical maximum range
math::Angle MultiRayShape::GetVerticalMaxAngle() const
{
  return this->vertElem->GetValueDouble("max_angle");
}
