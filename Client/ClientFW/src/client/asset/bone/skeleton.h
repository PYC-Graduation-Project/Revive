#pragma once
namespace client_fw
{
	class Skeleton : public std::enable_shared_from_this<Skeleton>//���߿� �ѹ��� �����ؾ��ҵ�
	{
	public:
		Skeleton() = default;
		virtual ~Skeleton() = default;

	private:
		//Ʈ���� �д°� �����ɸ��µ�..
		SPtr<Skeleton> m_parent = nullptr;
		SPtr<Skeleton> m_child = nullptr;
		SPtr<Skeleton> m_sibling = nullptr;

		std::string bone_name;

		Mat4 m_to_parent;

		
	public:

		Vec3 m_scale;
		Vec3 m_translation;
		Vec3 m_rotation;

	public:
		void SetChild(SPtr<Skeleton>& child, SPtr<Skeleton>& parent);

		void SetBoneName(const std::string& name) { bone_name = name; }

		void SetToParent(const Mat4& to_parent) { m_to_parent = to_parent; }

		const SPtr<Skeleton>& FindBone(const std::string& bone_name);
	};
}

