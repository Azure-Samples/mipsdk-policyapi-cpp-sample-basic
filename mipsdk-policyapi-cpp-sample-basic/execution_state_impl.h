/**
 *
 * Copyright (c) Microsoft Corporation.
 * All rights reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef SAMPLES_UPE_EXECUTION_STATE_IMPL_H_
#define SAMPLES_UPE_EXECUTION_STATE_IMPL_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "mip/protection_descriptor.h"
#include "mip/upe/action.h"
#include "mip/upe/execution_state.h"
#include "protection_descriptor_impl.h"

namespace sample {
	namespace policy {

		struct ExecutionStateOptions {
			std::unordered_map<std::string, std::string> metadata;
			std::shared_ptr<mip::Label> newLabel;
			std::string contentIdentifier;
			mip::ActionSource actionSource = mip::ActionSource::MANUAL;
			mip::DataState dataState = mip::DataState::USE;
			mip::AssignmentMethod assignmentMethod = mip::AssignmentMethod::STANDARD;
			bool isDowngradeJustified = false;
			std::string downgradeJustification;
			std::string templateId;
			std::string contentFormat;
			mip::ActionType supportedActions = mip::ActionType::REMOVE_WATERMARK;
			bool generateAuditEvent = true;
		};

		class ExecutionStateImpl final : public mip::ExecutionState {
		public:
			explicit ExecutionStateImpl(ExecutionStateOptions options) : mOptions(std::move(options)) {}

			std::shared_ptr<mip::Label> GetNewLabel() const override { return mOptions.newLabel; }			
			mip::DataState GetDataState() const override { return mOptions.dataState; }
			std::string GetContentIdentifier() const override { return mOptions.contentIdentifier; }			
			std::pair<bool, std::string> IsDowngradeJustified() const override {
				return std::make_pair(mOptions.isDowngradeJustified, mOptions.downgradeJustification);
			}			
			mip::AssignmentMethod GetNewLabelAssignmentMethod() const override { return mOptions.assignmentMethod; }			
			std::vector<std::pair<std::string, std::string>> GetNewLabelExtendedProperties() const override;
			std::vector<mip::MetadataEntry> GetContentMetadata(
				const std::vector<std::string>& names,
				const std::vector<std::string>& namePrefixes) const override;
			std::shared_ptr<mip::ProtectionDescriptor> GetProtectionDescriptor() const override {
				return std::make_shared<ProtectionDescriptorImpl>(mOptions.templateId);
			}
			std::string GetContentFormat() const override { return mOptions.contentFormat; };
			
			mip::ActionType GetSupportedActions() const override;
			std::shared_ptr<mip::ClassificationResults> GetClassificationResults(
				const std::vector<std::shared_ptr<mip::ClassificationRequest>>& /*classificationIds*/) const override;
			

		private:
			ExecutionStateOptions mOptions;
		};

	} //  namespace sample
} //  namespace upe

#endif //  SAMPLES_UPE_EXECUTION_STATE_IMPL_H_