/* Copyright (c) 2016-2017, NVIDIA CORPORATION. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  * Neither the name of NVIDIA CORPORATION nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define VMA_IMPLEMENTATION

#include <vkhlf/Allocator.h>
#include <vkhlf/Device.h>
#include <vkhlf/DeviceMemory.h>
#include <vkhlf/DeviceMemoryAllocator.h>
#include <vkhlf/DeviceMemoryChunk.h>
#include <vkhlf/PhysicalDevice.h>
#include <vkhlf/Instance.h>

namespace vkhlf
{

  DeviceMemoryAllocator::DeviceMemoryAllocator(Device& device, vkhlf::Instance& instance)
  {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_1;
    allocatorInfo.physicalDevice = static_cast<vk::PhysicalDevice>(*device.get<PhysicalDevice>());;
    allocatorInfo.device = static_cast<vk::Device>(device);
    allocatorInfo.instance = static_cast<vk::Instance>(instance);
    vmaCreateAllocator(&allocatorInfo, &m_allocator);
  }

  DeviceMemoryAllocator::~DeviceMemoryAllocator()
  {}

  DeviceMemoryAllocator::ImageInfo DeviceMemoryAllocator::createImage(vk::ImageCreateInfo imageInfo, bool hostVisible)
  {
    VkImage image;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
    VkImageCreateInfo imageCreateInfo = imageInfo;

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    if (hostVisible)
    {
      allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
      allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    }

    VK_VERIFY(static_cast<vk::Result>(vmaCreateImage(m_allocator, &imageCreateInfo, &allocCreateInfo, &image, &allocation, &allocationInfo)));

    return { std::make_shared<DeviceMemory>(m_allocator, allocation, allocationInfo), image };
  }

  DeviceMemoryAllocator::BufferInfo DeviceMemoryAllocator::createBuffer(vk::BufferCreateInfo bufferInfo, bool hostVisible)
  {
    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
    VkBufferCreateInfo buffInfo = bufferInfo;

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    if (hostVisible)
    {
      allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
      allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    }

    VK_VERIFY(static_cast<vk::Result>(vmaCreateBuffer(m_allocator, &buffInfo, &allocCreateInfo, &buffer, &allocation, &allocationInfo)));

    return { std::make_shared<DeviceMemory>(m_allocator, allocation, allocationInfo), buffer };
  }

} // namespace vkh
