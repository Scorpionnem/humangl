/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 16:04:06 by mbatty            #+#    #+#             */
/*   Updated: 2026/03/17 15:01:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Math.hpp"

class	Camera
{
	private:
		struct	Plane
		{
			float	A, B, C, D;
			void	normalize();
			Vec3d	getNormal()
			{
				return (Vec3d(A, B, C));
			}
			float	getDist(Vec3d point)
			{
				return (dot(getNormal(), point) + D);
			}
		};
		struct	Frustum
		{
			Plane top, bottom, right, left, zNear, zFar;
			bool	isInside(Vec3d minCorner, Vec3d maxCorner);
		};
	public:
		Camera() : Camera(Vec3d(0)) {}
		Camera(Vec3d pos)
		{
			this->pos = pos;
			front = Vec3d(0.0f, 0.0f, -1.0f);
			up = Vec3d(0.0f, 1.0f, 0.0f);
		}

		void	update(float delta, float aspectRatio);

		Mat4f	getViewMatrix()
		{
			return (lookAt<float>(Vec3d(0), Vec3d(0) + front, up));
		}

		float	yaw = 0;
		float	pitch = 0;
		Vec3d	pos;
		Vec3d	front;
		Vec3d	up;
		Frustum	frustum;
		float	speed = 0;
	private:
		void	_updatePlaneNormals(float aspectRatio);
		Vec3d	_direction;
		Vec3d	_lastPos;
};
