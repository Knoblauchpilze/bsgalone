#!/bin/bash

if [[ $# -ge 1 ]]; then
  VERSION=${1}
  if ! [[ $VERSION =~ ^v[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "Usage: ./publish.sh v1.2.3"
    exit 1
  fi

  echo "Using version from input: ${1}"
fi

if [[ $VERSION == "" ]]; then
  # Update local tags:
  # https://stackoverflow.com/questions/16678072/fetching-all-tags-from-a-remote-with-git-pull
  git fetch --tags

  # Try to pick the latest version:
  # https://stackoverflow.com/questions/6269927/how-can-i-list-all-tags-in-my-git-repository-by-the-date-they-were-created
  VERSION=$(git tag --sort=-creatordate | head -n 1)

  if [[ $VERSION == "" ]]; then
    echo "No versions defined yet, picking v0.0.0 as base revision"
    VERSION="v0.0.0"
  fi

  # Add a revision as per:
  # https://en.wikipedia.org/wiki/Software_versioning#Semantic_versioning
  BASE_VERSION=$(echo $VERSION | grep -Eo 'v[0-9]+\.[0-9]+\.[0-9]+')
  REVISION=$(echo $VERSION | cut -d. -f3)
  if [[ $REVISION == "" ]]; then
    REVISION="0"
  fi

  NEXT_REVISION=$(echo "${REVISION} + 1" | bc)
  TRIMMED_VERSION=$(echo ${BASE_VERSION} | grep -Eo 'v[0-9]+\.+[0-9]+\.')
  VERSION="${TRIMMED_VERSION}${NEXT_REVISION}"

  echo "No version provided in input, will go on with ${VERSION}"
fi

echo "Creating release ${VERSION}"
# https://stackoverflow.com/questions/18216991/create-a-tag-in-a-github-repository
git tag ${VERSION}

git push origin ${VERSION}
