/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "begin_native.hpp"
#include <GeodeTypeIdsImpl.hpp>
#include "end_native.hpp"

#include "ManagedCacheableDeltaBytes.hpp"
#include "../DataInput.hpp"
#include "../DataOutput.hpp"
#include "../CacheableString.hpp"
#include "../ExceptionTypes.hpp"
#include "SafeConvert.hpp"


using namespace System;

namespace apache
{
  namespace geode
  {
    namespace client
    {

      void ManagedCacheableDeltaBytesGeneric::toData(DataOutput& output) const
      {
        Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::toData: current domain ID: " + System::Threading::Thread::GetDomainID() + " for object: " + System::Convert::ToString((uint64_t) this) + " with its domain ID: " + m_domainId);
        try {
          output.writeBytesOnly(m_bytes, m_size);
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
      }

      void ManagedCacheableDeltaBytesGeneric::fromData(DataInput& input)
      {
        try {
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromData: classid " + m_classId);
          Apache::Geode::Client::DataInput mg_input(&input, true, input.getCache());
          const System::Byte* objStartPos = input.currentBufferPosition();

          Apache::Geode::Client::IGeodeSerializable^ obj =
            Apache::Geode::Client::Serializable::GetTypeFactoryMethodGeneric(m_classId)();
          obj->FromData(%mg_input);
          input.advanceCursor(mg_input.BytesReadInternally);

          m_hashCode = obj->GetHashCode();

          const System::Byte* objEndPos = input.currentBufferPosition();

          //m_size = mg_input.BytesRead;
          m_size = (System::UInt32)(objEndPos - objStartPos);
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromData: objectSize = " + m_size + " m_hashCode = " + m_hashCode);
          m_bytes = input.getBufferCopyFrom(objStartPos, m_size);
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
      }

      System::UInt32 ManagedCacheableDeltaBytesGeneric::objectSize() const
      {
        try {
          return m_size;
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        return 0;
      }

      System::Int32 ManagedCacheableDeltaBytesGeneric::classId() const
      {
        System::UInt32 classId;
        try {
          classId = m_classId;
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        return (classId >= 0x80000000 ? 0 : classId);
      }

      int8_t ManagedCacheableDeltaBytesGeneric::typeId() const
      {
        try {
          System::UInt32 classId = m_classId;
          if (classId >= 0x80000000) {
            return (int8_t)((classId - 0x80000000) % 0x20000000);
          }
          else if (classId <= 0x7F) {
            return (int8_t)GeodeTypeIdsImpl::CacheableUserData;
          }
          else if (classId <= 0x7FFF) {
            return (int8_t)GeodeTypeIdsImpl::CacheableUserData2;
          }
          else {
            return (int8_t)GeodeTypeIdsImpl::CacheableUserData4;
          }
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        return 0;
      }

      int8_t ManagedCacheableDeltaBytesGeneric::DSFID() const
      {
        // convention that [0x8000000, 0xa0000000) is for FixedIDDefault,
        // [0xa000000, 0xc0000000) is for FixedIDByte,
        // [0xc0000000, 0xe0000000) is for FixedIDShort
        // and [0xe0000000, 0xffffffff] is for FixedIDInt
        // Note: depends on fact that FixedIDByte is 1, FixedIDShort is 2
        // and FixedIDInt is 3; if this changes then correct this accordingly
        System::UInt32 classId = m_classId;
        if (classId >= 0x80000000) {
          return (int8_t)((classId - 0x80000000) / 0x20000000);
        }
        return 0;
      }

      bool ManagedCacheableDeltaBytesGeneric::hasDelta()
      {
        //Apache::Geode::Client::IGeodeDelta^ deltaObj = this->getManagedObject();
        //return deltaObj->HasDelta();
        return m_hasDelta;
      }

      void ManagedCacheableDeltaBytesGeneric::toDelta(DataOutput& output) const
      {
        try {
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::toDelta: current domain ID: " + System::Threading::Thread::GetDomainID() + " for object: " + System::Convert::ToString((uint64_t) this) + " with its domain ID: " + m_domainId);
          Apache::Geode::Client::IGeodeDelta^ deltaObj = this->getManagedObject();
          Apache::Geode::Client::DataOutput mg_output(&output, true);
          deltaObj->ToDelta(%mg_output);
          mg_output.WriteBytesToUMDataOutput();
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
      }

      void ManagedCacheableDeltaBytesGeneric::fromDelta(DataInput& input)
      {
        try {
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromDelta:");
          Apache::Geode::Client::IGeodeDelta^ deltaObj = this->getManagedObject();
          Apache::Geode::Client::DataInput mg_input(&input, true, input.getCache());
          deltaObj->FromDelta(%mg_input);

          Apache::Geode::Client::IGeodeSerializable^ managedptr =
            dynamic_cast <Apache::Geode::Client::IGeodeSerializable^> (deltaObj);
          if (managedptr != nullptr)
          {
            Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromDelta: current domain ID: " + System::Threading::Thread::GetDomainID() + " for object: " + System::Convert::ToString((uint64_t) this) + " with its domain ID: " + m_domainId);
            Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromDelta: classid " + managedptr->ClassId + " : " + managedptr->ToString());
            auto dataOut = input.getCache()->createDataOutput();
            Apache::Geode::Client::DataOutput mg_output(dataOut.get(), true);
            managedptr->ToData(%mg_output);

            //move cursor
            //dataOut.advanceCursor(mg_output.BufferLength);
            mg_output.WriteBytesToUMDataOutput();

            GF_SAFE_DELETE(m_bytes);
            m_bytes = dataOut->getBufferCopy();
            m_size = dataOut->getBufferLength();
            Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::fromDelta objectSize = " + m_size + " m_hashCode = " + m_hashCode);
            m_hashCode = managedptr->GetHashCode();
          }
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
      }

      DeltaPtr ManagedCacheableDeltaBytesGeneric::clone()
      {
        try {
          Apache::Geode::Client::IGeodeDelta^ deltaObj = this->getManagedObject();
          ICloneable^ cloneable = dynamic_cast<ICloneable^>((Apache::Geode::Client::IGeodeDelta^) deltaObj);
          if (cloneable) {
            Apache::Geode::Client::IGeodeSerializable^ Mclone =
              dynamic_cast<Apache::Geode::Client::IGeodeSerializable^>(cloneable->Clone());
            return DeltaPtr(static_cast<ManagedCacheableDeltaBytesGeneric*>(
              SafeMSerializableConvertGeneric(Mclone)));
          }
          else {
            return Delta::clone();
          }
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        return nullptr;
      }

      Apache::Geode::Client::IGeodeDelta^
        ManagedCacheableDeltaBytesGeneric::getManagedObject() const
      {

        Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytes::getManagedObject");

        auto dinp = m_cache->createDataInput(m_bytes, m_size);
        Apache::Geode::Client::DataInput mg_dinp(dinp.get(), true, m_cache);
        Apache::Geode::Client::TypeFactoryMethodGeneric^ creationMethod =
          Apache::Geode::Client::Serializable::GetTypeFactoryMethodGeneric(m_classId);
        Apache::Geode::Client::IGeodeSerializable^ newObj = creationMethod();

        Apache::Geode::Client::IGeodeDelta^ managedDeltaptr =
          dynamic_cast <Apache::Geode::Client::IGeodeDelta^> (newObj->FromData(%mg_dinp));
        return managedDeltaptr;
      }

      bool ManagedCacheableDeltaBytesGeneric::operator ==(const apache::geode::client::CacheableKey& other) const
      {
        try {
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal");
          // now checking classId(), typeId(), DSFID() etc. will be much more
          // expensive than just a dynamic_cast
          const ManagedCacheableDeltaBytesGeneric* p_other =
            dynamic_cast<const ManagedCacheableDeltaBytesGeneric*>(&other);
          if (p_other != NULL) {
            auto di = m_cache->createDataInput(m_bytes, m_size);
            Apache::Geode::Client::DataInput mg_input(di.get(), true, m_cache);
            Apache::Geode::Client::IGeodeSerializable^ obj =
              Apache::Geode::Client::Serializable::GetTypeFactoryMethodGeneric(m_classId)();
            obj->FromData(%mg_input);
            bool ret = obj->Equals(p_other->ptr());
            Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal return VAL = " + ret);
            return ret;
          }
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal returns false");
        return false;
      }

      bool ManagedCacheableDeltaBytesGeneric::operator ==(const ManagedCacheableDeltaBytesGeneric& other) const
      {
        try {
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal. ");
          auto di = m_cache->createDataInput(m_bytes, m_size);
          Apache::Geode::Client::DataInput mg_input(di.get(), true, m_cache);
          Apache::Geode::Client::IGeodeSerializable^ obj =
            Apache::Geode::Client::Serializable::GetTypeFactoryMethodGeneric(m_classId)();
          obj->FromData(%mg_input);
          bool ret = obj->Equals(other.ptr());
          Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal return VAL = " + ret);
          return ret;
          //return obj->Equals(other.get());
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        Apache::Geode::Client::Log::Debug("ManagedCacheableDeltaBytesGeneric::equal return false");
        return false;
      }

      System::Int32 ManagedCacheableDeltaBytesGeneric::hashcode() const
      {
        throw gcnew System::NotSupportedException;
      }

      size_t ManagedCacheableDeltaBytesGeneric::logString(char* buffer, size_t maxLength) const
      {
        try {
          Apache::Geode::Client::IGeodeDelta^ manageObject = getManagedObject();
          if (manageObject != nullptr)
          {
            if (maxLength > 0) {
              String^ logstr = manageObject->GetType()->Name + '(' +
                manageObject->ToString() + ')';
              Apache::Geode::Client::ManagedString mg_str(logstr);
              return snprintf(buffer, maxLength, "%s", mg_str.CharPtr);
            }
          }
        }
        catch (Apache::Geode::Client::GeodeException^ ex) {
          ex->ThrowNative();
        }
        catch (System::Exception^ ex) {
          Apache::Geode::Client::GeodeException::ThrowNative(ex);
        }
        return 0;
      }
    }  // namespace client
  }  // namespace geode
}  // namespace apache
