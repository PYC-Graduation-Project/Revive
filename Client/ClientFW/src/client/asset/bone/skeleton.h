#pragma once
namespace client_fw
{
	class Skeleton : public std::enable_shared_from_this<Skeleton>//나중에 한번더 정리해야할듯
	{
	public:
		Skeleton() = default;
		virtual ~Skeleton() = default;

	private:
		//트리가 읽는게 오래걸리는데..
		SPtr<Skeleton> m_parent = nullptr;
		SPtr<Skeleton> m_child = nullptr;
		SPtr<Skeleton> m_sibling = nullptr;

		std::string m_bone_name;

		Mat4 m_to_parent;
		Mat4 m_world;
		
	public:

		Vec3 m_scale;
		Vec3 m_translation;
		Vec3 m_rotation;

	public:
		const Mat4& GetWorld();

		const SPtr<Skeleton>& GetParent() { return m_parent; }
		const std::string& GetBoneName() { return m_bone_name; }

		void UpdateTransform(const Mat4& to_parent);

		void SetChild(SPtr<Skeleton>& child);

		void SetBoneName(const std::string& name) { m_bone_name = name; }

		void SetToParent(const Mat4& to_parent) { m_to_parent = to_parent; }

		const SPtr<Skeleton>& FindBone(const std::string& m_bone_name);
	};
}

